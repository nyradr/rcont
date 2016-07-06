/*	Rcont management
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include "rcont.h"
#include "relay.h"

#define BSIZE 10

Card* card;

void rcont_log(const char* mess, ...){
	FILE* file = fopen(RCONT_LOG, "a");
	
	if(file){
		va_list args;
		va_start(args, mess);
		
		fprintf(file, mess, args);
		fprintf(file, "\n");
		
		va_end(args);
		fclose(file);
	}
}



/*	Count the lines of a file
*/
unsigned int countline(FILE* file){
	unsigned int lines = 0;
	
	fseek(file, 0, SEEK_SET);
	
	while(!feof(file)){
		if(fgetc(file) == '\n')
			lines++;
	}
		
	return lines;
}

/* Load card from configuration file
*/
void cardFromFile(Card* card, FILE* file){
	// to file start
	fseek(file, 0, SEEK_SET);
	
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
				if(gpio > 0 && relay < card->relays_len){
					card_initrelay(card, relay, gpio, type, val);
					
					char log [126] = {0};
					sprintf(log, "Initialise relay %u with : %d gpio as %d type on %d val",
						relay, gpio, type, val);
					rcont_log(log);
					
					relay++;
				}
				// reset
				gpio = -1;
				type = RCONT_RTYPE_CONT;
				val = RCONT_RELAY_DOWN;
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

void rcont_update(){
	card_update(card, RCONT_DELAY);
}

int rcont_init(){
	// gpio init
	/*if(gpioInitialise() < 0){
		rcont_log("Rcont GPIO init error");
		exit(-1);
	}else
		rcont_log("Rcont GPIO init success");
	*/
	// read config
	FILE* file = fopen(RCONT_INFO, "r");
	if(file){
		unsigned int nline = countline(file);
		
		card = card_create(nline);
		
		char log[126] = {0};
		sprintf(log, "Initialise card with %u relays", nline);
		rcont_log(log);
		
		cardFromFile(card, file);
		
		fclose(file);
	}else{
		rcont_log("Rcont failed to load configuration file");
		exit(-1);
	}
}

void rcont_stop(){
	card_free(card);
	
	//gpioTerminate();
	rcont_log("Rcont daemon stop");
}
