CC = gcc
DEBUG = -ggdb
LFLAGS = -Wall -Werror
CFLAGS = $(LFLAGS) $(DEBUG) -std=c99

default: all

shell.o: shell.h shell.c
	$(CC) $(CFLAGS) -c shell.c

all: shell.o
	$(CC) $(CFLAGS) shell.o -o shell

clean:
	rm -rf *.o shell
