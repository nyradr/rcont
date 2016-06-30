/* Write some data into the rcont file (if exist)
*/ 

#include <stdio.h>

#define RCONT_FILE "/tmp/rcont/rcont.rcont"

int main(int argc, char** argv){
	if(argc == 3){
		FILE* file = fopen(RCONT_FILE, "a");
		
		if(file){
			fprintf(file, "%s %s\n", argv[1], argv[2]);
			fclose(file);
		}else
			printf("Cannot open file %s", RCONT_FILE);
	}else{
		printf("Invalid arguments : must be \"rcontwr relay delay\"\n");
		printf("\trelay from 1 - nrelays\n");
		printf("\tdelay delay before reset or -1\n");
	}
}
