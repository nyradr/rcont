#include <stdlib.h>
#include <unistd.h>
#include <pigpio.h>

#include "relay.h"
#include "files.h"

/*	Initialise relay
*/
void	relay_init(Relay* relay, char gpio,
	char type, char val){
	
	if(relay){
		relay->gpio = gpio;
		relay->type = type;
		relay->value = val;
		relay->delay = -1;
		
		gpioSetMode(relay->gpio, PI_OUTPUT);
		gpioWrite(relay->gpio, val);
	}
}

/*	Switch relay exit value
 * 	(0 -> 1; 1 -> 0)
*/
void relay_switch(Relay* relay){
	switch(relay->type){
		// continuous signal
		case RCONT_RTYPE_CONT:
			relay->value = (relay->value)?
				RCONT_RELAY_DOWN : RCONT_RELAY_UP;
			
			gpioWrite(relay->gpio, relay->value);
			break;
		
		// push signal and return to null
		case RCONT_RTYPE_PUSH:
			gpioWrite(relay->gpio, RCONT_RELAY_UP);
			sleep(RCONT_PUSHDELAY);
			gpioWrite(relay->gpio, RCONT_RELAY_DOWN);
			break;
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
		relay_init(&card->relays[relay], gpio, type, val);
	}
}

void card_free(Card* card){
	free(card->relays);
	free(card);
}

void card_switch(Card* card, unsigned int relay){
	if(relay < card->relays_len){
		relay_switch(&card->relays[relay]);
		writeCardState(card);
	}
}

void card_setDelay(Card* card, unsigned int relay, long delay){
	if(relay < card->relays_len){
		card->relays[relay].delay = delay;
		card_switch(card, relay);
	}
}

void card_update(Card* card, long delay){
	for(unsigned int i = 0; i < card->relays_len; i++){
		Relay* relay = &card->relays[i];
		
		if(relay->delay > 0){
			relay->delay -= delay;
			
			if(relay->delay < 0){
				relay->delay = -1;
				relay_switch(relay);
			}
		}
	}
}
