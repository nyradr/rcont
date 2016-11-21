/*	start relay controler deamon
 * 	Manage the deamon
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include "rcont.h"

/* Produce when the deamon is stoped
 */
void onexit(){
  rcont_stop();
 
  remove(RCONT_LOG);
  remove(RCONT_DIR);
}

/* When a process signal is handled
 */
void sighand(int sig){
  switch(sig){
  case SIGTERM:			
    exit(RCONT_EXIT_NONE);
    break;
		
  default:
    break;
  }
}

/* Create Rcont daemon	
 */
void daemonize(){
  int fid = fork();
  if(fid < 0){	// fork error
    printf("Rcont : fork fatal error\n");
    exit(RCONT_EXIT_FORK);
  }else if(fid > 0){ // parent
    exit(RCONT_EXIT_NONE);
  }else{
    // forked process
    setsid();

    struct stat dbuff;
    // change working dir
    if(stat(RCONT_DIR, &dbuff) < 0){
      printf("Creation of the rcont directory, %s\n", RCONT_DIR);
      if(mkdir(RCONT_DIR, 0777) != 0){
	printf("Unable to create rcont directory\n");
	exit(RCONT_EXIT_FORK);
      }
    }else
      printf("Rcont directory, %s, is already present\n", RCONT_DIR);
    
    if(chdir(RCONT_DIR) < 0){
      printf("Unable to set into the rcont directory\n");
      exit(RCONT_EXIT_FORK);
    }

    printf("Rcont daemonized\n");
    
    // close standars IO
    close( STDIN_FILENO );
    close( STDOUT_FILENO );
    close( STDERR_FILENO );
		
    // signal handler
    signal(SIGTERM, sighand);

    atexit(onexit);
  
    rcont_log("Rcont daemon start");
    rcont_init();
	
    while(1){
      rcont_update();
      usleep(RCONT_DELAY);
    }
  }
}


int main(int argc, char**argv){
  //as a daemon
  daemonize();
	
  return 0;
}
