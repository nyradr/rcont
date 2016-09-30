#ifndef RCONT_INC
#define RCONT_INC

#include "files.h"

// delay between scan (in s)
#define RCONT_DELAY 1

/*	Initialize rcont
*/
int rcont_init();

/*	Terminate Rcont
*/
void rcont_stop();

/*	Update rcont state
*/
void rcont_update();

#endif
