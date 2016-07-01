/*	Files constans and manipulation (log)
*/

#ifndef FILES_INC
#define FILES_INC

#define RCONT_DIR "/tmp/rcont"
#define RCONT_LOG "rcont.log"
#define RCONT_FILEIN "rcont.in"
#define RCONT_FILEOUT "rcont.out"
#define RCONT_INFO "/etc/rcont/rcont.conf"

/* Log into Rcont log file
 * The log file is defined by RCONT_LOG
 * mess	: message to log
*/
void rcont_log(const char* mess);

#endif
