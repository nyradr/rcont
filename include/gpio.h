#ifndef GPIO_INC
#define GPIO_INC

/* Open gpio port in output mode
   pin : GPIO pin name
   return : 0 in case of errors
   //TODO : value test (< 0)
*/
char gpio_init(int pin);

/* Close gpio port
   pin : GPIO pin name
   return : 0 in case of error
   //TODO : value test (< 0)
*/
char gpio_close(int pin);

/* Write value in GPIO port
   pin : GPIO pin name
   return : 0 in case of error
   //TODO : value test (< 0)
*/
char gpio_write(int pin, char val);

#endif
