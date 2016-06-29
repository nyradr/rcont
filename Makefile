rcont: main.c
	gcc -o rcont relay.c rcont.c main.c -lpigpio -lrt -pthread
