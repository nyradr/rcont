/*	Relay controler deamon
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "rcont.h"

#define RCONT_DIR "/tmp/rcont"
#define RCONT_LOG "rcont.log"

/*	Create Rcont daemon	
*/
void daemonize(){
	int fid = fork();
	if(fid < 0){	// fork error
		
		exit(-1);
	}else if(fid > 0){ // parent
		exit(0);
	}else{ // fork
		setsid();
		
		// change working dir
		if(stat(RCONT_DIR, NULL) < 0)
			mkdir(RCONT_DIR, 0777);
		chdir(RCONT_DIR);
		
		// close standars IO
		close( STDIN_FILENO );
		close( STDOUT_FILENO );
		close( STDERR_FILENO );
	}
}

int main(int argc, char**argv){	
	//as a daemon
	daemonize();
	
	//daemon
	while(1){}
	
	return 0;
}
