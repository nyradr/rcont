/* Write some data into the rcont file (if exist)
*/ 

#include <stdio.h>
#include <string.h>

#define DIR "/tmp/rcont/"
#define BSIZE 255

int main(int argc, char** argv){
	char buff[BSIZE] = {0};
	FILE* file;
	
	switch(argc){
		case 3:
			sprintf(buff, "%s%d.in", DIR, argv[1]);
			
			file = fopen(buff, "a");
			if(file){
				fprintf(file, "%d\n", argv[2]);
				fclose(file);
			}else
				printf("Impossible to open the file : %s", buff);
		
			break;
			
		default:
			printf("Invalid arguments : must be \"rcontwr relay delay\"\n");
			printf("\trelay from 1 - nrelays\n");
			printf("\tdelay delay before reset or 0\n");
			break;
	}
}
