/*	Relay controler
*/

#include <stdio.h>
#include <pigpio.h>

#DEFINE PORT 4

int main(int argc, char**argv){
	if(gpioInitialize() < 0){
		return -1;
	}
	
	gpioSetMode(PORT, PI_OUTPUT);
	
	gpioWrite(PORT, 1);
	
	gpioTerminate();
	
	return 0;
}
