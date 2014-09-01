#
# SecureKB - An input encryption daemon for Linux (using an external tool)
#
# Copyleft 2014 Oussema HARBI <oussema.elharbi@gmail.com>
#
# This program is free software; you can redistribute it and/or modify it
# whith ZERO restrictions.
#

prefix := /usr/local
sbindir := $(prefix)/bin

DEBUG := -g
CFLAGS := -O2 -Wall $(DEBUG)


VER := $(shell head -n 1 TODO | cut -d : -f 2)



all: SecureKB

SecureKB: core.c TODO
	$(CC) $(CFLAGS) -DVERSION=\"$(VER)\" $< -o $@

install: all
	install -D -m755 evmapd $(sbindir)/SecureKB

clean:
	rm -f *.o SecureKB
