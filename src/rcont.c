/*	Rcont management
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rcont.h"
#include "relay.h"

#define BSIZE 10

Card* card;

/* Count the lines of a file
 */
unsigned int countline(FILE* file){
  unsigned int lines = 0;
	
  fseek(file, 0, SEEK_SET);
	
  while(!feof(file)){
    if(fgetc(file) == '\n')
      lines++;
  }
		
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
				
	  rcont_log("Initialise relay %u with : %d gpio as %d type on %d value",
		    relay, gpio, type, val);
					
	  relay++;
	}
	// reset
	gpio = -1;
	type = RCONT_RTYPE_CONT;
	val = RCONT_RELAY_DOWN;
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

void rcont_update(){
  card_update(card, RCONT_DELAY);
}

int rcont_init(){
  // read config
  FILE* file = fopen(RCONT_INFO, "r");
  if(file){
    unsigned int nline = countline(file);
		
    card = card_create(nline);
		
    char log[126] = {0};
    rcont_log("Initialise card with %u relay", nline);
		
    cardFromFile(card, file);
		
    fclose(file);
  }else{
    rcont_log("Failed to load configuration file");
    exit(RCONT_EXIT_FILE);
  }
}

void rcont_stop(){
  card_free(card);
  rcont_log("Daemon stop");
}
