#ifndef RCONT_INC
#define RCONT_INC

// daemon constants
#define RCONT_DIR "/tmp/rcont"
#define RCONT_LOG "rcont.log"
#define RCONT_FILE "rcont.rcont"
#define RCONT_INFO "/etc/rcont/rcont.conf"

// delay between scan
#define RCONT_DELAY 1

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
