/* Write some data into the rcont file (if exist)
*/ 

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DIR "/tmp/rcont/"
#define BSIZE 255

/* Write the relay switch
   relay : relay name
   delay : delay before switch
 */
void write_relay(int relay, int delay){
  // init relay file
  char buff[BSIZE] = {0};
  if(snprintf(buff, BSIZE, "%s%d.in", DIR, relay) > 0){

    // open FIFO file
    int fd = open(buff, O_WRONLY);
    if(fd < 0){

      //write data
      if(write(fd, &delay, sizeof(int)) == sizeof(int))
	printf("Success\n");
      else
	printf("Error in delay write\n");
    }else
      printf("Error in relay file opening\n");
  }else
    printf("Error in relay file initialization\n");
}

int main(int argc, char** argv){
  char buff[BSIZE] = {0};
  FILE* file;
	
  switch(argc){
  case 3:
    snprintf(buff, BSIZE, "%s%s.in", DIR, argv[1]);
			
    file = fopen(buff, "a");
    if(file){
      fprintf(file, "\n%s", argv[2]);
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
