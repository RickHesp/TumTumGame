#ifndef SEGMENT_H_
#define SEGMENT_H_

#include <avr/io.h>
#include <util/delay.h>
#include "TWI.h"

#ifdef __cplusplus
extern "C" {
#endif


void expander_write(uint8_t data, uint8_t expander);
int countDown_step(uint8_t expander, int digit);
void clear_display(uint8_t expander);

#ifdef __cplusplus
}
#endif


#endif