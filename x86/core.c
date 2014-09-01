/* DISCLAIMER :
 * This is heavilly based on evampd which is a deamon for keystroke remapping running in deamon mode 
 * Code is dirty, many parts can still be removed (I kept them because i was thinking about making a custom 3D mouse .. not for now)
 * License is "No License" eeventhough it's based on some GPLV2 code --> OK, am not following Rules, SUE ME !!
 */

#define UINPUT_DEVICE		"/dev/uinput"   //This is the path to the uinput device, check if it's correct in your case
#define INPUT_DEVICE		"/dev/input/by-path/platform-i8042-serio-0-event-kbd"	//This is the path to my keyboard input device file 

#define DEBUG			1	//Activating debug

#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>
#include <termios.h>	//Needed  for the term data structures


#define msg(m, ...)		info("%s: " m, argv0, ##__VA_ARGS__)

#define RETERR(c, E, r, m, ...)	if (c) { \
					int e = (E)?(r):(errno); \
					msg(m ": %s\n", ##__VA_ARGS__ , strerror(e)); \
					return e; \
				}
#define RETERN(c, m, ...)	RETERR(c, 0, 0, m, ##__VA_ARGS__)

#define cfree(p)		if (p != NULL) free(p);



static int detach = 0, grab = 1, log = 0, quiet = 0, verbose = 0;



static char *argv0, *idev = NULL, *pidfile = NULL;

static int ifp = -1, ofp = -1;
static int kkm[2];
static int **nm = NULL;

#define ARR(a, c, x, y)		((a)[((x) * (c)) + (y)])

#define KKM(x, y)		ARR(kkm, 2, (x), (y))

int info(const char *fmt, ...)
{
	va_list args;
	int ret = 0;

	va_start(args, fmt);
	if (fileno(stderr) >= 0)
		ret = vfprintf(stderr, fmt, args);
	if (log > 1)
		vsyslog(LOG_NOTICE, fmt, args);
	va_end(args);

	return ret;
}

/* PID file creation */
static int write_pid() {
	FILE *fp;

	fp = fopen(pidfile, "w");
	if (fp == NULL)
		return -1;

	fprintf(fp, "%i\n", getpid());

	if (verbose)
		info("Wrote PID file %s\n", pidfile);

	fclose(fp);

	return 0;
}

static void rfree(void **v)
{
	int i;

	if (v == NULL)
		return;

	for (i = 0; v[i] != NULL; ++i)
		free(v[i]);

	free(v);
}

/* Allow SIGTERM to cause graceful termination */
void on_term(int s)
{
	int ret;

	if (detach)
		info("evmapd %s terminating for %s\n", VERSION, idev);

	if (grab) {
		ret = ioctl(ifp, EVIOCGRAB, (void *)0);
		if (ret != 0)
			msg("Warning: could not release %s\n", idev);
	}

	if (log)
		closelog();
	close(ifp);
	close(ofp);

	cfree(idev);
	//cfree(kkm);
	rfree((void **)nm);

	if (pidfile != NULL) {
		unlink(pidfile);
		free(pidfile);
	}

	exit(0);

	return;
}



#define VERTRIPLET(v)		(v >> 15), (v >> 8) & 0xff, (v & 0xff)

#define USAGE	"keyrouter Version " VERSION

static int usage(int r)
{
	info(USAGE);
	return r;
}

#define EV_EV			0

#define LEN(t, b)		(((b - 1) / (sizeof(t) * 8)) + 1)
#define POS(c, b)		(b / (sizeof((c)[0]) * 8))
#define OFF(c, b)		(b % (sizeof((c)[0]) * 8))
#define GET(c, b)		((c[POS(c, b)] >> OFF(c, b)) & 1)
#define SET(c, b, v)		(c)[POS(c, b)] = (((c)[POS(c, b)] & ~(1 << OFF(c, b))) | (((v) > 0) << OFF(c, b)))

#if DEBUG
#define INQ(i, m)		ret = ioctl(ifp, EVIOC##i, m); \
				RETERN(ret < 0, "Unable to query input device %s (EVIOC" #i ") [%i]", idev, __LINE__)
#define OSET(i, m)		ret = ioctl(ofp, UI_##i, m); \
				RETERN(ret < 0, "Unable to configure output device %s (UI" #i ") [%i]", odev, __LINE__)
#else
#define INQ(i, m)		ret = ioctl(ifp, EVIOC##i, m); \
				RETERN(ret < 0, "Unable to query input device %s (EVIOC" #i ")", idev)
#define OSET(i, m)		ret = ioctl(ofp, UI_##i, m); \
				RETERN(ret < 0, "Unable to configure output device %s (UI" #i ")", odev)
#endif

#define OSETBIT(t)		for (i = 0; i < t##_MAX; ++i) \
					if GET(obits[EV_##t], i) \
						OSET(SET_##t##BIT, i); \

#define NONEG(x)		if (x < 0) x = 0;

#define AC			ac[ev.code]

static void listbits(unsigned long evbits[EV_MAX][LEN(long, KEY_MAX)], int bits, int max, char *dsc)
{
	int i, j = 0;

	if GET(evbits[0], bits) {
		info("\t%s:\n", dsc);
		for (i = 0; i < max; ++i) {
			if GET(evbits[bits], i) {
				if ((j % 8) == 0)
					info("\t");
				info("\t%d", i);
				++j;
				if ((j % 8) == 0)
					info("\n");
			}
		}
		info("\n%s", ((j % 8) != 0)?"\n":"");
	}
}

int main(int argc, char **argv)
{
	int help = 0, version = 0;

	char *odev = UINPUT_DEVICE;

	int iver;
	char iphys[256];
	struct uinput_user_dev uidev = {};
	unsigned long ibits[EV_MAX][LEN(long, KEY_MAX)];

	char ophys[256] ;
	struct uinput_user_dev uodev = {};
	unsigned long obits[EV_MAX][LEN(long, KEY_MAX)];

	int i, j, ret, counter;
	unsigned long rbits[EV_MAX][LEN(long, KEY_MAX)];
	struct input_event ev;


	argv0 = argv[0];
	char* idev = INPUT_DEVICE;

	
	int USB;
	struct termios tty;

	char chTx='\0';
	char chRx='\0';
	USB = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );

	memset (&tty, 0, sizeof tty);

	/* Error Handling */
	if ( tcgetattr ( USB, &tty ) != 0 )
	{
		printf("Error from tcgetattr: %s \n",strerror(errno));
	}

	/* Set Baud Rate */
	cfsetospeed (&tty, (speed_t)B9600);
	cfsetispeed (&tty, (speed_t)B9600);

	/* Setting other Port Stuff */
	tty.c_cflag     &=  ~PARENB;        // Make 8n1
	tty.c_cflag     &=  ~CSTOPB;
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;

	tty.c_cflag     &=  ~CRTSCTS;       // no flow control
	tty.c_cc[VMIN]      =   1;                  // read doesn't block
	tty.c_cc[VTIME]     =   5;                  // 0.5 seconds read timeout
	tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

	/* Make raw */
	cfmakeraw(&tty);

	/* Flush Port, then applies attributes */
	tcflush( USB, TCIFLUSH );
	if ( tcsetattr ( USB, TCSANOW, &tty ) != 0)
	{
		printf("Error from tcsetattr\n");
	}

	if (help)
		return usage(0);
	if (version) {
		info("evmapd Version " VERSION "\n");
		return 0;
	}
	if (idev == NULL) {
		msg("No input device specified\n\n");
		return usage(EINVAL);
	}

	/* Map parsing */
	//STRINT(kkmap, kkm, "%i:%i", 2);
	kkm[0] = 37;
	kkm[1] = 38;

	/* Open the syslog facility */
	if (log == 1) {
		openlog("evmapd", LOG_PID, LOG_DAEMON);
		log = 2;
	}
	if (quiet && !detach) {
		fclose(stdin);
		fclose(stdout);
		fclose(stderr);
	}

	/* Open the input device */
	ifp = open(idev, O_RDONLY);
	RETERN(ifp < 0, "Unable to open input device %s", idev);

	/* Open the output device */
	ofp = open(odev, O_WRONLY);
	RETERN(ofp < 0, "Unable to open output device %s", odev);
	
	/* Grab the input device */
	if (grab) {
		ret = ioctl(ifp, EVIOCGRAB, (void *)1);
		RETERN(ret < 0, "Unable to grab input device %s", idev);
	}


	/* Get the input device information */
	memset(ibits, 0, sizeof(ibits));
	memset(rbits, 0, sizeof(rbits));
	memset(obits, 0, sizeof(obits));

	INQ(GVERSION, &iver);
	INQ(GID, &(uidev.id));
	INQ(GNAME(sizeof(uidev.name)), uidev.name);
	INQ(GPHYS(sizeof(iphys)), iphys);
	INQ(GBIT(0, EV_MAX), ibits[0]);

	for (i = 1; i < EV_MAX; ++i) {
		if GET(ibits[0], i) {
			ioctl(ifp, EVIOCGBIT(i, LEN(long, KEY_MAX) * sizeof(long)), ibits[i]);

			if (i == EV_ABS) {
				for (j = 0; j < ABS_MAX; ++j) {
					if GET(ibits[i], j) {
						struct input_absinfo abs;

						INQ(GABS(j), &abs);
						uidev.absmax[j] = abs.maximum;
						uidev.absmin[j] = abs.minimum;
						uidev.absfuzz[j] = abs.fuzz;
						uidev.absflat[j] = abs.flat;
					}
				}
			}
		}
	}

	/* Print input device information */
	if (verbose) {
		info("Input device: %s\n"
		    "\tName: %s\n"
		    "\tPhys: %s\n"
		    "\tBus: %u / Vendor: %u / Product: %u / Version: %u.%u.%u / Driver: %d.%d.%d\n"
		    "\n",
		    idev,
		    uidev.name,
		    iphys,
		    uidev.id.bustype, uidev.id.vendor, uidev.id.product, VERTRIPLET(uidev.id.version), VERTRIPLET(iver)
		);

		info("\tEvent types:");
		for (i = 1; i < EV_MAX; ++i)
			if GET(ibits[0], i)
				info(" %i", i);
		info("\n\n");

		listbits(ibits, EV_KEY, KEY_MAX, "KEY");
		info("\n");
	}


	/* The output device information */
	memset(&(uodev.id), 0, sizeof(uodev.id));
	snprintf(ophys, sizeof(ophys), "evmapd/%i", getpid());
	
	uodev = uidev;

	if (kkm != NULL) {
		SET(obits[EV_EV], EV_KEY, 1);
		for (i = 0; KKM(i, 0) != -1; ++i)
			if GET(ibits[EV_KEY], KKM(i, 0)) {
				SET(rbits[EV_KEY], KKM(i, 0), 1);
				SET(obits[EV_KEY], KKM(i, 1), 1);
			}
	}

	/* Do not let through the remapped event bits */	
	for (i = 0; i < EV_MAX; ++i)
		for (j = 0; j < LEN(long, KEY_MAX); ++j)
			obits[i][j] |= (ibits[i][j] & ~rbits[i][j]);

	/* Print output device information */
	if (verbose) {
		info("Output device: %s\n"
		    "\tName: %s\n"
		    "\tPhys: %s\n"
		    "\tBus: %u / Vendor: %u / Product: %u / Version: %u.%u.%u\n"
		    "\n",
		    odev,
		    uodev.name,
		    ophys,
		    uodev.id.bustype, uodev.id.vendor, uodev.id.product, VERTRIPLET(uodev.id.version)
		);

		info("\tEvent types:");
		for (i = 1; i < EV_MAX; ++i)
			if GET(obits[0], i)
				info(" %i", i);

		info("\n\n");

		listbits(obits, EV_KEY, KEY_MAX, "KEY");

		info("\n");
	}


	/* Prepare the output device */
	OSET(SET_PHYS, ophys);
	OSETBIT(EV);
	OSETBIT(KEY);
	OSETBIT(REL);
	OSETBIT(ABS);
	OSETBIT(MSC);
	OSETBIT(LED);
	OSETBIT(SND);
	OSETBIT(FF);
	OSETBIT(SW);

	ret = write(ofp, &uodev, sizeof(uodev));
	RETERR(ret < (int)(sizeof(uodev)), ret >= 0, EIO, "Unable to configure output device %s", odev);
	OSET(DEV_CREATE, NULL);


	/* Daemon mode */
	if (detach) {
		ret = daemon(0, 0);
		RETERN(ret < 0, "Could not run in the background");
		info("evmapd %s launched for %s using %s for output (PID: %i)\n",
				VERSION, idev, odev, getpid());
	}

	/* PID file support */
	if (pidfile != NULL) {
		ret = write_pid();
		RETERN(ret < 0, "Could not write PID file %s", pidfile);
	}

	/* Setup the signal handlers */
	signal(SIGTERM, on_term);


#define _RCV			ret = read(ifp, &ev, sizeof(ev)); \
				RETERR(ret < (int)(sizeof(ev)), ret >= 0, EIO, "Unable to receive event from %s", idev);

#define _SND			ret = write(ofp, &ev, sizeof(ev)); \
				RETERR(ret < (int)(sizeof(ev)), ret >= 0, EIO, "Unable to send event to %s", odev); \
				if (ev.type == EV_KEY) SET(rbits[EV_KEY], ev.code, ev.value);

#if DEBUG
#define RCV			_RCV \
				if (verbose) \
					info("IN: %6i %6i %6i\n", ev.type, ev.code, ev.value);
#define SND			if (verbose) \
					info("OUT: %6i %6i %6i\n", ev.type, ev.code, ev.value); \
				_SND
#else
#define RCV			_RCV
#define SND			_SND
#endif

	/* The event loop */
	memset(rbits, 0, sizeof(rbits));
	counter = 0;
	ev.type = EV_KEY;
	ev.code = 28;
	ev.value = 1;
	SND;
	while (1) {
		RCV;
		/* Event processing */
		if (ev.type == EV_KEY) {
				if (ev.code > 30 && ev.code < 40) {
					chTx = ev.code;
					i = write( USB, (char*)&chTx, 1 );
					usleep(5000);
					i = read( USB, (char*)&chRx, 1);
					ev.code = chRx;
					counter++;
				}
		}
		SND;
		if (counter > 50)
			break;
	}

	close(USB);
	return 0;
}
