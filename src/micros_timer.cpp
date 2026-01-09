#include <avr/io.h>
#include <avr/interrupt.h>
#include "micros_timer.h"

#define TIMER1_OVERFLOW_VALUE 65536
volatile uint32_t t1_overflows = 0;

uint32_t micros_timer(void){
  //combine timer overflows and current timer1 value to get microseconds.
  //with prescaler 8, each tick is 0.5uS
  uint32_t ticks = (t1_overflows*TIMER1_OVERFLOW_VALUE) + (uint32_t)TCNT1;//65536 is the max for 16bit.
  return ticks /2;//0.5 uS per tick
}

uint8_t one_second_passed(){
  if(t1_overflows >= 31){
    t1_overflows = 0;
    return 1;
  }else{return 0;}
}

uint8_t micros_ms_timer_delay(uint32_t delay_time){
  //delay in microseconds
  uint32_t start = micros_timer();
  while((micros_timer() - start) < delay_time){
    //wait
  }
}

ISR(TIMER1_OVF_vect){
  t1_overflows++;
}