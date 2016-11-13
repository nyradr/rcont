/* Write some data into the rcont file (if exist)
*/ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
  if(relay <= 0){
    printf("Invalid relay");
    return;
  }
  
  // init relay file
  char buff[BSIZE] = {0};
  if(snprintf(buff, BSIZE, "%s%d.in", DIR, relay) > 0){

    // open FIFO file
    int fd = open(buff, O_WRONLY);
    if(fd >= 0){

      //write data
      if(write(fd, &delay, sizeof(int)) > 0)
	printf("Success\n");
      else
	printf("Error in delay write\n");

      close(fd);
    }else
      printf("Error in relay file opening\n");
  }else
    printf("Error in relay file initialization\n");
}

int main(int argc, char** argv){
  char buff[BSIZE] = {0};
  FILE* file;
	
  if(argc == 3){
    int relay = strtol(argv[1], NULL, 10);
    int delay = strtol(argv[2], NULL, 10);
    write_relay(relay, delay);    
  }else{
    printf("Invalid arguments : must be \"rcontwr relay delay\"\n");
    printf("\trelay from 1 - nrelays\n");
    printf("\tdelay delay before switch, 0 for instant switch\n");
    printf("\tnegative delay for reset or all commands\n");
  }
}
