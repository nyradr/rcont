/*	Rcont management
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pigpio.h>

#include "rcont.h"
#include "relay.h"

Card* card;

/*	Count the lines of a file
*/
unsigned int countline(FILE* file){
	unsigned int lines;
	
	fseek(file, 0, SEEK_SET);
	
	while(EOF != (scanf("%*[^\n]"), scanf("%*c")))
		lines++;
		
	return lines;
}

/* Load card from configuration file
*/
void cardFromFile(Card* card, FILE* file){
	// to file start
	fseek(file, 0, SEEK_SET);
	
	#define BSIZE 3
	
	// relay values
	char gpio = -1;
	char type = RCONT_RTYPE_CONT;
	char val = RCONT_RELAY_DOWN;
	char state = 0;
	unsigned int relay = 0;
	
	// buffer
	char buff[BSIZE] = {0};
	char ibuff = 0;
	
	// read all the file
	while(!feof(file)){
		char c = fgetc(file);
		if(c == '\n' || c == '\t' || c == ' '){ // end of line or value
			switch(state){
				case 0:	// gpio value
					gpio = atoi(buff);
					break;
				case 1: // type value
					type = atoi(buff);
					if(	type != RCONT_RTYPE_PUSH &&
						type != RCONT_RTYPE_PUSH )
						type = RCONT_RTYPE_CONT;
					break;
				case 2: // init GPIO state
					val = atoi(buff);
					if( val != RCONT_RELAY_DOWN &&
						val != RCONT_RELAY_UP )
						val = RCONT_RELAY_DOWN;
					break;
			}
			// reset buffer, next state
			state++;
			memset(&buff, 0, BSIZE);
			ibuff = 0;
			
			// end of line or end values
			if(state > 3 || c == '\n'){
				if(gpio >= 0 && relay < card->relays_len){
					card_initrelay(card, relay, gpio, type, val);
				}
				// reset
				gpio = -1;
				type = RCONT_RTYPE_CONT;
				val = RCONT_RELAY_DOWN;
				relay++;
				state = 0;
			}
		}else{
			// add to buffer
			if(ibuff < BSIZE){
				buff[ibuff] = c;
				ibuff++;
			}
		}
	}
}

void rcont_log(const char* mess){
	FILE* file = fopen(RCONT_LOG, "a");
	
	if(file){
		fprintf(file, "%s\n", mess);
		fclose(file);
	}
}

void tfunct(){
	for(unsigned int i = 0; i < card->relays_len; i++)
		card_switch(card, i);
}

int rcont_init(){
	const unsigned char PORT [PORT_S] = {PORT_1, PORT_2, PORT_3, PORT_4};
	
	if(gpioInitialise() < 0){
		rcont_log("Rcont GPIO init error");
		exit(-1);
	}else
		rcont_log("Rcont GPIO init success");
	
	FILE* file = fopen(RCONT_INFO, "r");
	if(file){
		unsigned int nline = countline(file);
		
		card = card_create(nline);
		cardFromFile(card, file);
	
		gpioSetTimerFunc(0, 1000, tfunct);
	}else{
		rcont_log("Rcont failed to load configuration file");
		exit(-1);
	}
}

void rcont_stop(){
	card_free(card);
	
	gpioTerminate();
	rcont_log("Rcont daemon stop");
}
