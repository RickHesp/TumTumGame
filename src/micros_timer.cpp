#include <avr/io.h>
#include <avr/interrupt.h>
#include "micros_timer.h"

volatile uint32_t t1_overflows = 0;

uint32_t micros_timer(void){
  //combine timer overflows and current timer1 value to get microseconds.
  //with prescaler 8, each tick is 0.5uS
  uint32_t ticks = (t1_overflows*65536) + (uint32_t)TCNT1;//65536 is the max for 16bit.
  return ticks /2;//0.5 uS per tick
}

ISR(TIMER1_OVF_vect){
  t1_overflows++;
}