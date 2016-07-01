CC=gcc
CFLAGS=-std=c11
LIBS=-lpigpio -lrt -pthread

all: rcont rcontwr

clean:
	rm rcont rcontwr

rcontwr:
	$(CC) -o rcontwr rcontwr.c

rcont: main.c
	$(CC) -o rcont relay.c rcont.c main.c $(LIBS) $(CFLAGS)
