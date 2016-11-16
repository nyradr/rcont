/*	Relay card management
 */
#ifndef RELAY_INC
#define RELAY_INC

/* Relay in continuous mode
   Relay value is inverted until the next inversion
*/
#define RCONT_RTYPE_CONT	1
/* Relay in push mode
   Relay value is inverted for RCONT_PUSHDELAY seconds
   and go back to the previous value
*/
#define RCONT_RTYPE_PUSH	2

/* Delay (in ms) of the relay push inverstion
 */
#define RCONT_PUSHDELAY		1000

/* Value to write in the GPIO as UP value
 */
#define RCONT_RELAY_UP		1
/* Value to write in the GPIO as DOWN value
 */
#define RCONT_RELAY_DOWN	0

/* Represent a programmed switch as a linked list
 */
struct Switch{
  /* Next programmed switch (or NULL if nothing programmed)
   */
  struct Switch* next;
  unsigned int delay;		// in delay (s)
};
typedef struct Switch Switch;

/* Represent a relay
 */
struct Relay{
  /* Relay name (unique ID)
   */
  unsigned int name;
  /* GPIO port name
   */
  char	gpio;
  /* GPIO type (see RCONT_TYPE_?
   */
  char	type;
  /* Actual GPIO value
   */
  char	value;
  /* Nexts programmed switchs
   */
  Switch*	next;
  /* Last programmed switch
     ptr on the last element of next linked list
   */
  Switch* last;
	
  /* Command input file
     File address (absolute)
     Used to take new switch to programs
   */
  char*	in;
  /* Status output file
     File address (absolute)
     Used to show current status and programmed switchs
   */
  char*	out;
  /* Flag to tell if the relay value is changed
     and not writed in the output file
     (1 if changed, 0 otherwise)
   */
  char	changed;
	
	
};
typedef struct Relay Relay;

/* Represent the relay card
   (list of relays)
 */
struct Card{
  /* Array of all relays
   */
  Relay*	relays;
  /* Array size
   */
  unsigned int relays_len;
};
typedef struct Card Card;

/* Create the relay card
   nrelay : number of relays
   return : new cart ptr (to free) or NULL
 */
Card* card_create(unsigned int nrelay);

/* Initialise a relay on a card
   card : relay card ptr
   relay : relay name (unique ID)
   gpio : GPIO pin name
   type : see RCONT_TYPE_?
   val : initial value to write on GPIO
 */
void card_initrelay(Card* card, unsigned int relay,
		    char gpio, char type, char val);

/* Free card and all relays
   card : relay card ptr
 */
void card_free(Card* card);

/* Update the delays of the card and make the switch
   card : relay card ptr
   delay : delay to elapse
 */
void card_update(Card* card, unsigned int delay);


#endif
