#include "files.h"

#include <stdio.h>
#include <stdarg.h>

void rcont_log(const char* mess, ...){
  FILE* file = fopen(RCONT_LOG, "a");
	
  if(file){
    va_list args;
    va_start(args, mess);
		
    vfprintf(file, mess, args);
    fprintf(file, "\n");
		
    va_end(args);
    fclose(file);
  }
}
