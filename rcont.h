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
#define PORT	{PORT_1, PORT_2, PORT_3, PORT_4}
#define PORT_S	4


/* Log into Rcont log file
 * The log file is defined by RCONT_LOG
 * mess	: message to log
*/
void rcont_log(const char* mess);

/*	Initialize rcont
*/
int rcont_init();

/*	Terminate Rcont
*/
void rcont_stop();

#endif
