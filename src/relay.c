#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "relay.h"
#include "files.h"
#include "gpio.h"

#define BSIZE 8


/* Initialise relay
   relay : relay ptr
   name : relay unique ID
   gpio : GPIO pin name
   type : see RCONT_TYPE_?
   val : initial value to write on GPIO
   int case of errors the program will exit
 */
void	relay_init(Relay* relay, unsigned int name,
		   char gpio, char type, char val){
	
  if(relay){
    relay->name = name;
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
      rcont_log("Malloc error");
      exit(-1);
    }
    sprintf(relay->in, "%d.in\0", name);
    if(mkfifo(relay->in, 0600) != 0){
      rcont_log("Fifo creation error for relay%d", relay->name);
      exit(-1);
    }
		
    // output file
    relay->out = malloc(BSIZE * sizeof(char));
    if(!relay->out){
      rcont_log("Malloc error");
      exit(-1);
    }
    sprintf(relay->out, "%d.out\0", name);
    fclose(fopen(relay->out, "w"));
  }
}

/* Clear all programmed commands in the relay
   relay : relay
 */
void relay_cleancmd(Relay* relay){
  while(relay->next != NULL){
    Switch* sw = relay->next;
    relay->next = relay->next->next;
    free(sw);
  }
}

/* Close relay gpio and delete IO files
   relay : relay
 */
void relay_close(Relay* relay){
  relay_cleancmd(relay);
  remove(relay->in);
  remove(relay->out);
  gpio_close(relay->gpio);
}

/* Execute programmed switch
   (0 -> 1; 1 -> 0)
   Load next switch
 */
void relay_switch(Relay* relay){
  // new value
  relay->value = (relay->value)?
    RCONT_RELAY_DOWN : RCONT_RELAY_UP;
  relay->changed = 1;
  
  // load next switch after this switch
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
	
  rcont_log("Relay %u switched", relay->name);
}

/* Write current relay state inside out file
   Also write the current relay program
   Set changed state to 0
 */
void relay_out(Relay* relay){
  if(relay->changed){
    FILE* fout = fopen(relay->out, "w");
		
    unsigned int delay = 0;
    if(relay->next != NULL)
      delay = relay->next->delay;
		
    if(fout){
      // print actual state
      fprintf(fout, "%d", relay->value);
			
      // print program
      Switch* sw = relay->next;
			
      while(sw != NULL){
	fprintf(fout, " %d", sw->delay);
	sw = sw->next;
      }
			
      fclose(fout);
      relay->changed = 0;
    }
  }
}

/* Read input file and execute command
 */
void relay_in(Relay* relay){
  int fd = open(relay->in, O_RDONLY);
	
  if(fd < 0){
    // read all commands
    int d = 0;
    while(read(fd, &d, sizeof(int)) == sizeof(int)){
      if(d >= 0){
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
				
	rcont_log("Relay %u reading new command", relay->name);		
      }else{
	// no valid data, reset
	relay_cleancmd(relay);
	if(relay->value == RCONT_RELAY_UP)
	  relay_switch(relay);
						
	rcont_log("Relay %u commands cleanup", relay->name); 
      }
    }
    close(fd);
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
