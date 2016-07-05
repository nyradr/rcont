#ifndef GPIO_INC
#define GPIO_INC

/*	Open gpio port in output mode
*/
char gpio_init(int pin);

/*	Close gpio port
*/
char gpio_close(int pin);

/*	Write value in GPIO port
*/
char gpio_write(int pin, char val);

#endif
