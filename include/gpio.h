#ifndef GPIO_INC
#define GPIO_INC

/* Open gpio port in output mode
   pin : GPIO pin name
   return : 1 if no error
            0 if an error occur
*/
char gpio_init(int pin);

/* Close gpio port
   pin : GPIO pin name
   return : 1 if no error
            0 if an error occur
*/
char gpio_close(int pin);

/* Write value in GPIO port
   pin : GPIO pin name
   return : 1 if no error
            0 if an error occur
*/
char gpio_write(int pin, char val);

#endif
