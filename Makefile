rcont: rcont.c
	gcc -o rcont rcont.c -W -Wall -lpigpio -lrt -pthread
