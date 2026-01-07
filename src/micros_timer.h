#ifndef MICROS_TIMER_H
#define MICROS_TIMER_H

#include <avr/io.h>


//enable micros timer
uint32_t micros_timer();

//delay in microseconds using micros timer
uint8_t micros_ms_timer_delay(uint32_t delay_us);

uint8_t one_second_passed();

#endif