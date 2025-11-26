//countdown functions
#include <avr/io.h>


void TWI_setup(void);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8_t data);
void expander_write(uint8_t data);
int countDown_start(void);