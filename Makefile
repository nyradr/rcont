rcont: rcont.c
	gcc -o rcont rcont.c  -lpigpio -lrt -pthread
