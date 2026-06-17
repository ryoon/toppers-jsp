
# Makefile for bin2img.

CC = gcc

CFLAGS = -O3 -Wall

SHELL = /bin/sh

# This rule allows us to supply the necessary -D options
# in addition to whatever the user asks for.

all:  bin2uf2

bin2uf2: bin2uf2.c Makefile
	${CC} ${CFLAGS} $@.c -I. -o $@

clean:
	rm -f *.o core bin2uf2

distclean: clean
	rm -f *.c~ Makefile~
