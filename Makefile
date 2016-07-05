CC=gcc
CFLAGS=-std=c11
LIBS=

all: rcont rcontwr

clean:
	rm rcont rcontwr

rcontwr:
	$(CC) -o rcontwr rcontwr.c

rcont: main.c
	$(CC) -o rcont gpio.c relay.c rcont.c main.c $(LIBS) $(CFLAGS)

install:
	mkdir -p /etc/rcont
	cp rcont.conf /etc/rcont/rcont.conf
	cp rcont /bin/rcont
	chmod 755 /bin/rcont
	cp rcontwr /bin/rcontwr	
	chmod 755 /bin/rcontwr
	#cp rcontd.sh /etc/init.d/rcontd.sh
	#chmod 755 /etc/init.d/rcontd.sh
	#update-rc.d rcontd.sh default

uninstall:
	rm -r /etc/rcont
	rm /bin/rcont /bin/rcontwr
	#update-rc.d rcontd.sh remove
	#rm /etc/init.d/rcontd.sh
