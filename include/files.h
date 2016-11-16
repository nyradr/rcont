/*	Files constans and manipulation (log)
*/

#ifndef FILES_INC
#define FILES_INC

#define RCONT_DIR "/tmp/rcont"
#define RCONT_LOG "rcont.log"
#define RCONT_INFO "/etc/rcont/rcont.conf"

#define RCONT_EXIT_NONE 0
#define RCONT_EXIT_MALLOC -1
#define RCONT_EXIT_GPIO -2
#define RCONT_EXIT_FILE -3
#define RCONT_EXIT_FORK -4

/* Log into Rcont log file
 * The log file is defined by RCONT_LOG
 * mess	: message to log
*/
void rcont_log(const char* mess, ...);

#endif
