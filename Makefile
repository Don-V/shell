CC = gcc
DEBUG = -ggdb
LFLAGS = -Wall -Werror
CFLAGS = $(LFLAGS) $(DEBUG) -std=gnu99

default: all

main.o: shell.h main.c utils.h
	$(CC) $(CFLAGS) -c main.c

shell.o: shell.h shell.c builtins.h utils.h
	$(CC) $(CFLAGS) -c shell.c

builtins.o: builtins.h builtins.c utils.h
	$(CC) $(CFLAGS) -c builtins.c

utils.o: utils.h utils.c
	$(CC) $(CFLAGS) -c utils.c

list.o: linked_list/list.h linked_list/list.c
	$(CC) $(CFLAGS) -c linked_list/list.c

all: shell.o builtins.o utils.o main.o list.o
	$(CC) $(CFLAGS) builtins.o main.o shell.o utils.o list.o -o shell

clean:
	rm -rf *.o shell
