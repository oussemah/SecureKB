SecureKB
========

This project is about using an MCU board (any MCU that's able to communicate through UART) to encrypt keyboard strokes before they are handled by the operating system.

keyboard ---> /dev/input --> SecureKB --> uart-STM32 ---> SecureKB --> /dev/event --> OS

In this precise example I ll provide the MCU code for an STM32L and an ARDUINO code.
The x86 part should be the same (or almost) no matter the MCU used.

I tested this on my Toshiba laptop hosting a Linux Mint 16.

x86 part
========

To compile the x86 part, go to the x86 folder, and type make.
I assume you have a gcc compiler, build-essential, i don't remember what else but i believe those 2 should be enough.
Drop me a line if you find a library or a package is needed to compile correctly.

You ll need to have the uinput and evdev modules available (they are implicitly available in 2.6+ kernels i believe)

mcu part
========

Use your preferred dev environment to build the code (or port it to your custom mcu..etc) then load it to your mcu.
The provided code was compiled using arm-none-eabi-gnu toolchain , latest version available in August 2014 .

Make sure you have the toolchain installed and added to your path.
Then go to the Std-blablabla/Debug folder , then type make.
Do the same with USART_Polling/Debug folder ;
You guessed right : This is based on a default project package for eclipse with very minor modifications.

I use arm-none-eabi-objcopy -Obinary to get the binary from elf.
Then i used openocd to flasht hat on the stm32 starting from offset 0x8000000

execution
=========
conned the mcu on a USB port (throug a serial to usb cable may be)

load modules evdev and uinput

start the x86 part (with sudo rights)   // You need to check that the path used in core.c for input is actually the same //in your system, otherwise you need to update it inside core.c and recompile
//Same remarq goes for the serial port path


Drop me a word if you need any help with any of the steps above.


Please feel free to comment, update and especially SHARE.

Author: Oussema HARBI <oussema.elharbi@gmail.com>
