#ifndef RCONT_INC
#define RCONT_INC

#include "files.h"

/* delay between scan (in ms)
 */
#define RCONT_DELAY 500

/* Initialize rcont
*/
int rcont_init();

/* Terminate Rcont
*/
void rcont_stop();

/* Update rcont state
   Execute programmed commands
   Read new commands
   Write card state
*/
void rcont_update();

#endif
