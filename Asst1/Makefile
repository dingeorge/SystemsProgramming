all: mymalloc memgrind

CC = gcc
CFLAGS = -g -Wall -Werror

memgrind: mymalloc memgrind.c
	$(CC) -g -o memgrind mymalloc.o memgrind.c

mymalloc:
	$(CC) $(CFLAGS) -c mymalloc.c mymalloc.h

.PHONY = clean

clean:
	rm -rf *.o *.gch memgrind
