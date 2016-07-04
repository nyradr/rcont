/*	Relay card management
*/
#ifndef RELAY_INC
#define RELAY_INC

#define RCONT_RTYPE_CONT	1
#define RCONT_RTYPE_PUSH	2

// delay in s
#define RCONT_PUSHDELAY		1

#define RCONT_RELAY_UP		1
#define RCONT_RELAY_DOWN	0

/*	Represent a relay
*/
struct Relay{
	unsigned int name;
	// GPIO number
	char	gpio;
	// GPIO type (see RCONT_TYPE_?
	char	type;
	// actual GPIO value
	char	value;
	// delay before next switch
	unsigned int delay;
	// command input file
	char*	in;
	// status output file
	char*	out;
	// relay state changed (1 if changed, 0 else)
	char	changed;
	
};
typedef struct Relay Relay;

/*	Represent the relay card
*/
struct Card{
	Relay*	relays;
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
	char gpio, char type, char val);

/*	Free card and relays
*/
void card_free(Card* card);

/*	Switch relay output
 * 	If the relay is PUSH the value will return to null after PUSHDELAY ms
*/
void card_switch(Card* card, unsigned int relay);

/*	Switch the relay and set the delay time before next switch
*/
void card_setDelay(Card* card, unsigned int relay, long delay);

/*	Update the delays of the card and make the switch
*/
void card_update(Card* card, long delay);

/*	Write relay state into RCONT_FILEOUT
*/
void card_writeState(Card* card);

#endif
