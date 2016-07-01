CC=gcc
CFLAGS=-std=c11
LIBS=-lpigpio -lrt -pthread

all: clean rcont rcontwr

clean:
	rm rcont rcontwr

rcontwr:
	$(CC) -o rcontwr rcontwr.c

rcont: main.c
	$(CC) -o rcont relay.c rcont.c main.c $(LIBS) $(CFLAGS)

install:
	sudo mkdir -p /etc/rcont
	sudo cp rcont.conf /etc/rcont/rcont.conf
	sudo cp rcontd.sh /etc/init.d/rcontd.sh
	sudo chmod 755 /etc/init.d/rcontd.sh
	sudo update-rd.d rcontd.sh default
