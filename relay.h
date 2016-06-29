/*	Relay card management
*/
#ifndef RELAY_INC
#define RELAY_INC

#define RCONT_RTYPE_CONT	1
#define RCONT_RTYPE_PUSH	2

#define RCONT_PUSHDELAY		200

/*	Represent a relay
*/
struct Relay{
	// GPIO number
	unsigned char	gpio;
	// GPIO type (see RCONT_TYPE_?
	unsigned char	type;
	// actual GPIO value
	unsigned char	value;
	// delay before next switch
	unsigned long	delay;
};
typedef struct Relay Relay;

/*	Represent the relay card
*/
struct Card{
	Relay**	relays;
	unsigned int relays_len;
};
typedef struct Card Card;

/* Create the relay card
 * nrelay : number of relays
*/
Card* card_create(unsigned int nrelay);

/*	Initialise a relay
*/
void card_initrelay(Card* card, unsigned int relay,
	unsigned char gpio, unsigned char type, unsigned char val);

/*	Free card and relays
*/
void card_free(Card* card);

/*	Switch relay output
 * 	If the relay is PUSH the value will return to null after PUSHDELAY ms
*/
void card_switch(Card* card, unsigned int relay);


#endif
