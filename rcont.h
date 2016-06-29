#ifndef RCONT_INC
#define RCONT_INC

// daemon constants
#define RCONT_DIR "/tmp/rcont"
#define RCONT_LOG "rcont.log"
#define RCONT_FILE "rcont.rcont"

#define PORT_1	4
#define PORT_2	11
#define PORT_3	13
#define PORT_4	15

/* Log into Rcont log file
 * The log file is defined by RCONT_LOG
 * mess	: message to log
*/
void rcont_log(const char* mess);

int rcont_init();

int rcont_update();

void rcont_stop();

#endif
