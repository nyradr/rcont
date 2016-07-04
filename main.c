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

/*	Produce when the deamon is stoped
*/
void onexit(){
	rcont_stop();
	
	remove(RCONT_LOG);
	remove(RCONT_DIR);
}

/*	When a process signal is handled
*/
void sighand(int sig){
	switch(sig){
		case SIGTERM:			
			exit(0);
			break;
		
		default:
			break;
	}
}

/*	Create Rcont daemon	
*/
void daemonize(){
	int fid = fork();
	if(fid < 0){	// fork error
		printf("Rcont : fork fatal error\n");
		exit(-1);
	}else if(fid > 0){ // parent
		exit(0);
	}else{ // fork
		setsid();
		
		// change working dir
		if(stat(RCONT_DIR, NULL) < 0){
			mkdir(RCONT_DIR, 0777);
			chdir(RCONT_DIR);
		}
		else{
			printf("Rcont : change dir error");
			exit(-1);
		}
		
		// close standars IO
		close( STDIN_FILENO );
		close( STDOUT_FILENO );
		close( STDERR_FILENO );
		
		signal(SIGTERM, sighand);
	}
}


int main(int argc, char**argv){	
	//as a daemon
	daemonize();
	
	//daemon
	rcont_log("Rcont daemon start");
	rcont_init();
	atexit(onexit);
	while(1){
		
	}
	
	return 0;
}
