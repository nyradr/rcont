#include <stdlib.h>
#include <unistd.h>
#include <pigpio.h>
#include <stdio.h>
#include <string.h>

#include "relay.h"
#include "files.h"

#define BSIZE 8

/*	Initialise relay
*/
void	relay_init(Relay* relay, unsigned int name,
	char gpio, char type, char val){
	
	if(relay){
		relay->name = name;
		relay->gpio = gpio;
		relay->type = type;
		relay->value = val;
		relay->delay = -1;
		
		// init gpio
		gpioSetMode(relay->gpio, PI_OUTPUT);
		gpioWrite(relay->gpio, val);
		
		// create files
		
		// input file
		relay->in = malloc(BSIZE * sizeof(char));
		if(!relay->in){
			rcont_log("Malloc error\n");
			exit(-1);
		}
		sprintf(relay->in, "%d.in\0", name);
		
		// output file
		relay->out = malloc(BSIZE * sizeof(char));
		if(!relay->out){
			rcont_log("Malloc error\n");
			exit(-1);
		}
		sprintf(relay->out, "%d.out\0", name);
	}
}

/*	Switch relay exit value
 * 	(0 -> 1; 1 -> 0)
*/
void relay_switch(Relay* relay, unsigned int delay){
	switch(relay->type){
		// continuous signal
		case RCONT_RTYPE_CONT:
			relay->value = (relay->value)?
				RCONT_RELAY_DOWN : RCONT_RELAY_UP;
			
			gpioWrite(relay->gpio, relay->value);
			break;
		
		// push signal and return to previous value
		case RCONT_RTYPE_PUSH:
			gpioWrite(relay->gpio, RCONT_RELAY_UP);
			sleep(RCONT_PUSHDELAY);
			gpioWrite(relay->gpio, RCONT_RELAY_DOWN);
			break;
	}
	
	relay->delay = delay;
	changed = 1;
}

/*	Write current relay state inside out file
 * 	Set changed state to 0
*/
void relay_out(Relay* relay){
	if(relay->changed){
		FILE* fout = fopen(relay->out, "w");
		
		if(fin){
			fprintf(fout, "%d %d", relay->value, relay->delay);
			fclose(fout);
			changed = 0;
		}
	}
}

/*	Read input file and execute command
*/
void relay_in(Relay* relay){
	FILE* fin = fopen(relay->in, "r");
	
	if(fin){
		// read all commands
		while(!feof(fin)){
			unsigned int d = 0;
			if(fscanf(fin, "%d", &d) == 1){
				relay_switch(relay, d);
			}
		}
	}
}

Card* card_create(unsigned int nrelay){
	Card* card = malloc(sizeof(Card));
	
	if(card){
		card->relays_len = nrelay;
		card->relays = malloc(sizeof(Relay) * nrelay);
		
		if(!card->relays){
			free(card);
			exit(-1);
		}
	}else
		exit(-1);
		
	return card;
}

void card_initrelay(Card* card, unsigned int relay,
	char gpio, char type, char val){

	if(relay < card->relays_len){
		relay_init(&card->relays[relay], relay, gpio, type, val);
	}
}

void card_free(Card* card){
	free(card->relays);
	free(card);
}

void card_switch(Card* card, unsigned int relay){
	if(relay < card->relays_len){
		relay_switch(&card->relays[relay]);
		card_writeState(card);
	}
}

void card_setDelay(Card* card, unsigned int relay, long delay){
	if(relay < card->relays_len){
		card->relays[relay].delay = delay;
		card_switch(card, relay);
	}
}

void card_update(Card* card, long delay){
	char nswitch = 0;
	
	for(unsigned int i = 0; i < card->relays_len; i++){
		Relay* relay = &card->relays[i];
		
		if(relay->delay > 0){
			relay->delay -= delay;
			
			if(relay->delay <= 0){
				relay->delay = -1;
				relay_switch(relay);
				nswitch++;
			}
		}
	}
	
	if(nswitch > 0)
		card_writeState(card);
}

void card_writeState(Card* card){
	FILE* file = fopen(RCONT_FILEOUT, "w");
	
	if(file){
		for(unsigned int i = 0; i < card->relays_len; i++){
			fprintf(file, "%u %d %ds\n", i, card->relays[i].value, card->relays[i].delay);
		}
		fclose(file);
	}
}
