/*	Rcont management
*/

#include <stdio.h>
#include <stdlib.h>

#include <pigpio.h>

#include "rcont.h"
#include "relay.h"

void rcont_log(const char* mess){
	FILE* file = fopen(RCONT_LOG, "a");
	
	if(file){
		fprintf(file, "%s\n", mess);
		fclose(file);
	}
}

Card* card;

int rcont_init(){
	const unsigned char PORT [PORT_S] = {PORT_1, PORT_2, PORT_3, PORT_4};
	
	if(gpioInitialise() < 0){
		rcont_log("Rcont GPIO init error");
		exit(-1);
	}else
		rcont_log("Rcont GPIO init success");
		
	card = card_create(PORT_S);
	for(unsigned int i = 0; i < PORT_S; i++)
		card_initrelay(card, i,
			PORT[i], RCONT_RTYPE_CONT, RCONT_RELAY_DOWN);
}

void rcont_stop(){
	gpioTerminate();
	rcont_log("Rcont daemon stop");
}
