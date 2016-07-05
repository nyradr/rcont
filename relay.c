#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "relay.h"
#include "files.h"
#include "gpio.h"

#define BSIZE 8


/*	Initialise relay
*/
void	relay_init(Relay* relay, unsigned int name,
	char gpio, char type, char val){
	
	if(relay){
		relay->gpio = gpio;
		relay->type = type;
		relay->value = val;
		relay->next = NULL;
		relay->changed = 1;
		
		// init gpio
		gpio_init(relay->gpio);
		gpio_write(relay->gpio, val);
		
		// create files
		
		// input file
		relay->in = malloc(BSIZE * sizeof(char));
		if(!relay->in){
			rcont_log("Malloc error\n");
			exit(-1);
		}
		sprintf(relay->in, "%d.in\0", name);
		fclose(fopen(relay->in, "w"));
		
		// output file
		relay->out = malloc(BSIZE * sizeof(char));
		if(!relay->out){
			rcont_log("Malloc error\n");
			exit(-1);
		}
		sprintf(relay->out, "%d.out\0", name);
		fclose(fopen(relay->out, "w"));
	}
}

/*	Close relay gpio and delete IO files
*/
void relay_close(Relay* relay){
	remove(relay->in);
	remove(relay->out);
	gpio_close(relay->gpio);
}

/*	Switch relay exit value
 * 	(0 -> 1; 1 -> 0)
 * 	Load next switch
*/
void relay_switch(Relay* relay){
	relay->value = (relay->value)?
		RCONT_RELAY_DOWN : RCONT_RELAY_UP;
	relay->changed = 1;
	
	// load next next switch
	if(relay->next != NULL){
			Switch* sw = relay->next;
			relay->next = relay->next->next;
			free(sw);
	}
	
	switch(relay->type){
		// continuous signal
		case RCONT_RTYPE_CONT:
			gpio_write(relay->gpio, relay->value);
			break;
		
		// push signal
		case RCONT_RTYPE_PUSH:
			gpio_write(relay->gpio, RCONT_RELAY_UP);
			sleep(RCONT_PUSHDELAY);
			gpio_write(relay->gpio, RCONT_RELAY_DOWN);
			break;
	}
}

/*	Write current relay state inside out file
 * 	Set changed state to 0
*/
void relay_out(Relay* relay){
	if(relay->changed){
		FILE* fout = fopen(relay->out, "w");
		
		unsigned int delay = 0;
		if(relay->next != NULL)
			delay = relay->next->delay;
		
		if(fout){
			fprintf(fout, "%d %d", relay->value, delay);
			fclose(fout);
			relay->changed = 0;
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
			if(fscanf(fin, "%u", &d) == 1){
				// push switch to command list
				Switch* sw = malloc(sizeof(Switch));
				if(!sw){
					rcont_log("Malloc error");
					exit(-1);
				}
				sw->next = NULL;
				sw->delay = d;
				
				if(relay->next == NULL){
					relay->next = sw;
					relay->last = sw;
				}else{
					relay->last->next = sw;
					relay->last = sw;
				}
			}else{ // no valid data
				if(!feof(fin)){
					fseek(fin, 0, SEEK_END);
					rcont_log("No valid input data");
				}
			}
		}
		
		fclose(fin);
		fclose(fopen(relay->in, "w"));
	}
}

/*	Update the relay
*/
void relay_update(Relay* relay, unsigned int elapsed){
	// elasped time management
	if(relay->next != NULL){
		if(relay->next->delay > elapsed){
			relay->next->delay -= elapsed;
			relay->changed = 1;
		}else
			relay_switch(relay);
	}
	
	// new entry
	relay_in(relay);
	relay_out(relay);
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
		relay_init(&card->relays[relay], relay +1, gpio, type, val);
	}
}

void card_free(Card* card){
	for(unsigned int i = 0; i < card->relays_len; i++){
		Relay* relay = &card->relays[i];
		relay_close(relay);
	}
	
	free(card->relays);
	free(card);
}

void card_update(Card* card, unsigned int delay){
	for(unsigned int i = 0; i < card->relays_len; i++){
		Relay* relay = &card->relays[i];
		
		relay_update(relay, delay);
	}
}
