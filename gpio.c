#include <stdio.h>
#include <stdlib.h>

#include "gpio.h"
#include "files.h"

#define GPIO_LEN 256
#define GPIO_BASE "/sys/class/gpio/gpio"
#define GPIO_FEXPORT "/sys/class/gpio/export"
#define GPIO_FUEXPORT "/sys/class/gpio/unexport"

char gpio_init(int pin){
	
	FILE* file = fopen(GPIO_FEXPORT, "w");
	if(file){
		fprintf(file, "%d", pin);
		fclose(file);
	}else{
		rcont_log("Failed init gpio %d", pin);
		exit(-1);
	}
	
	char buff[GPIO_LEN] = {0};
	snprintf(buff, GPIO_LEN, "%s%d/direction", GPIO_BASE, pin);
	
	FILE* dir = fopen(buff, "w");
	if(dir){
		fprintf(dir, "out");
		fclose(dir);
	}else{
		rcont_log("Failed init gpio %d direction", pin);
		gpio_close(pin);
		exit(-1);
	}
	
	rcont_log("Gpio %d init success", pin);
	
	return file != 0 && dir != 0;
}

char gpio_close(int pin){
	
	FILE* file = fopen(GPIO_FUEXPORT, "w");
	if(file){
		fprintf(file, "%d", pin);
		fclose(file);
	}
	
	return file != 0;
}

char gpio_write(int pin, char val){
	char buff[GPIO_LEN] = {0};
	snprintf(buff, GPIO_LEN, "%s%d/value", GPIO_BASE, pin);
	
	FILE* file = fopen(buff, "w");
	if(file){
		fprintf(file, "%d", (int) val);
		fclose(file);
	}
	
	return file != 0;
}
