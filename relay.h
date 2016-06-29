#ifnef RELAY_INC
#define RELAY_INC

#define RCONT_RTYPE_CONT	1
#define RCONT_RTYPE_PUSH	2

struct Relay{
	unsigned char	port;
	unsigned char	type;
};
typedef struct Relay Relay;

void	relay_switch();

struct Card{
	Relay**	relays;
	unsigned int relays_len;
};
typedef struct Card Card;

Card* card_create(int nrelay);

void card_free(Card* card);

void card_switch(Card* card, unsigned int relay);


#endif
