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

install:
	sudo mkdir -p /etc/rcont
	sudo cp rcont.conf /etc/rcont/rcont.conf
	sudo cp rcont /bin/rcont
	chmod 755 /bin/rcont
	sudo cp rcontwr /bin/rcontwr	
	chmod 755 /bin/rcontwr
	#sudo cp rcontd.sh /etc/init.d/rcontd.sh
	#sudo chmod 755 /etc/init.d/rcontd.sh
	#sudo update-rc.d rcontd.sh default

uninstall:
	sudo rm -r /etc/rcont
	sudo rm /bin/rcont /bin/rcontwr
	#sudo update-rc.d rcontd.sh remove
	#sudo rm /etc/init.d/rcontd.sh
