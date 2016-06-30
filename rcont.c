/*	Rcont management
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pigpio.h>

#include "rcont.h"
#include "relay.h"

#define BSIZE 10

Card* card;

void rcont_log(const char* mess){
	FILE* file = fopen(RCONT_LOG, "a");
	
	if(file){
		fprintf(file, "%s\n", mess);
		fclose(file);
	}
}

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
					sprintf(log, "Initialise relay with : %d gpio as %d type on %d val", gpio, type, val);
					rcont_log(log);
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

/*	Update relay status
*/
void update(){
	FILE* file = fopen(RCONT_FILE, "r");
	
	card_update(card, RCONT_DELAY);
	
	// test file
	if(file){
		char buff [BSIZE] = {0};
		char ibuff = 0;
		char relay = -1;
		char delay = -1;
		
		// read file
		while(!feof(file)){
			char c = fgetc(file);
			if(c == ' '){
				relay = atoi(buff);
				memset(&buff, 0, BSIZE);
				ibuff = 0;
			
			}else if (c == '\n'){
				delay = atoi(buff);
				
				if(relay > 0 && relay <= card->relays_len){
					relay--;
					card_setDelay(card, relay, delay);
					char log[126];
					sprintf(log, "Switch relay %d for %d s", relay, delay);
					rcont_log(log);
				}
				
				memset(&buff, 0, BSIZE);
				ibuff = 0;
				relay = -1;
				delay = -1;
			
			}else if (ibuff < BSIZE){
				buff[ibuff] = c;
				ibuff++;
			}
		}
		
		fclose(file);
		// delete file content
		truncate(RCONT_FILE, 0);
	}else{
		rcont_log("Cannot open file");
	}
}

int rcont_init(){
	
	if(gpioInitialise() < 0){
		rcont_log("Rcont GPIO init error");
		exit(-1);
	}else
		rcont_log("Rcont GPIO init success");
	
	FILE* file = fopen(RCONT_INFO, "r");
	if(file){
		unsigned int nline = countline(file);
		
		card = card_create(nline);
		char log[126] = {0};
		sprintf(log, "Initialise card with %d relays", nline);
		rcont_log(log);
		
		cardFromFile(card, file);
	
		gpioSetTimerFunc(0, RCONT_DELAY * 1000, update);
		
		fclose(file);
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
