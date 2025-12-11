#include <avr/io.h>
#include <util/delay.h>
#include "TWI.h"

void expander_write(uint8_t data, uint8_t expander);
int countDown_start(uint8_t expander);
int countDown_step(uint8_t expander, int digit);