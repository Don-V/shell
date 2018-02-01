CC = gcc
DEBUG = -ggdb
LFLAGS = -Wall -Werror
CFLAGS = $(LFLAGS) $(DEBUG) -std=gnu99

default: all

shell.o: shell.h shell.c builtins.h
	$(CC) $(CFLAGS) -c shell.c

builtins.o: builtins.h builtins.c
	$(CC) $(CFLAGS) -c builtins.c

utils.o: utils.h utils.c
	$(CC) $(CFLAGS) -c utils.c

all: shell.o builtins.o utils.o
	$(CC) $(CFLAGS) builtins.o shell.o utils.o -o shell

clean:
	rm -rf *.o shell
