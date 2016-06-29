/*	Rcont management
*/

#include <stdio.h>
#include <stdlib.h>

#include "rcont.h"

void rcont_log(const char* mess){
	FILE* file = fopen(RCONT_LOG, "a");
	
	if(file){
		fprintf(file, "%s\n", mess);
		fclose(file);
	}
}

int rcont_init(){
	
	
	
}

void rcont_stop(){
	rcont_log("Rcont daemon stop");
}
