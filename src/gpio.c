#include <stdio.h>
#include <stdlib.h>

#include "gpio.h"

#define GPIO_LEN 256
#define GPIO_BASE "/sys/class/gpio/gpio"
#define GPIO_FEXPORT "/sys/class/gpio/export"
#define GPIO_FUEXPORT "/sys/class/gpio/unexport"

char gpio_init(int pin){	
  FILE* file = fopen(GPIO_FEXPORT, "w");
  
  if(file){
    fprintf(file, "%d", pin);
    fclose(file);

    char buff[GPIO_LEN] = {0};
    if(snprintf(buff, GPIO_LEN, "%s%d/direction", GPIO_BASE, pin) > 0){
      
      FILE* fdir = fopen(buff, "w");
      if(fdir){
	fprintf(fdir, "out");
	fclose(fdir);
	return 1;
      }else
	gpio_close(pin);
    }
  }
  
  return 0;
}

char gpio_close(int pin){
  FILE* file = fopen(GPIO_FUEXPORT, "w");
  if(file){
    fprintf(file, "%d", pin);
    fclose(file);
    return 1;
  }

  return 0;
}

char gpio_write(int pin, char val){
  char buff[GPIO_LEN] = {0};
  if(snprintf(buff, GPIO_LEN, "%s%d/value", GPIO_BASE, pin) > 0){
    FILE* file = fopen(buff, "w");
    if(file){
      fprintf(file, "%d", (int) val);
      fclose(file);
      return 1;
    }
  }
  return 0;
}
