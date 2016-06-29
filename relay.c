#include <stdlib.h>
#include <pigpio.h>

#include "relay.h"

/*	Initialise relay
*/
void	relay_init(Relay* relay, unsigned char gpio,
	unsigned char type, unsigned char val){
	
	if(relay){
		relay->gpio = gpio;
		relay->type = type;
		relay->value = val;
		relay->delay = 0;
		
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
			relay->value = (relay->value)? 0 : 1;
			gpioWrite(relay->gpio, relay->value);
			break;
		
		// push signal and return to null
		case RCONT_RTYPE_PUSH:
			gpioTrigger(relay->gpio, RCONT_PUSHDELAY, 1);
			break;
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
	unsigned char gpio, unsigned char type, unsigned char val){

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
	}
}
