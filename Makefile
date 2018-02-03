CC = gcc
DEBUG = -ggdb
LFLAGS = -Wall -Werror
CFLAGS = $(LFLAGS) $(DEBUG) -std=gnu99

default: all

mian.o: shell.h
	$(CC) $(CFLAGS) -c main.c

shell.o: shell.h shell.c builtins.h utils.h
	$(CC) $(CFLAGS) -c shell.c

builtins.o: builtins.h builtins.c utils.h
	$(CC) $(CFLAGS) -c builtins.c

utils.o: utils.h utils.c
	$(CC) $(CFLAGS) -c utils.c

all: shell.o builtins.o utils.o main.o
	$(CC) $(CFLAGS) builtins.o main.o shell.o utils.o -o shell

clean:
	rm -rf *.o shell
