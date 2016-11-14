CC=gcc
CFLAGS=-std=c11
INC=-I./include
SRC=./src
OBJ=./obj
BIN=./bin

all: rcont rcontwr

rcontwr: rcontwr.o
	$(CC) -o $(BIN)/rcontwr $(INC) $(CFLAGS) $(OBJ)/rcontwr.o

rcont: main.o files.o gpio.o relay.o rcont.o
	$(CC) -o $(BIN)/rcont $(INC) $(CFLAGS) $(OBJ)/gpio.o $(OBJ)/relay.o $(OBJ)/rcont.o $(OBJ)/main.o $(OBJ)/files.o

%.o : $(SRC)/%.c
	$(CC) -o $(OBJ)/$@ -c $(CFLAGS) $(INC) $^

clean:
	rm -f $(OBJ)/* $(BIN)/*

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
