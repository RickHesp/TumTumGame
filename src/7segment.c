#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "TWI.h"
#include "7segment.h"
#include "micros_timer.h"

const uint8_t digit_map[10] = {
    0b10000000, // 0
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000010, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10010000  // 9
};

void expander_write(uint8_t data, uint8_t expander) {
    TWI_start();
    TWI_write(expander << 1);
    TWI_write(data);
    TWI_stop();
}

int countDown_step(uint8_t expander, int digit) {
    expander_write(digit_map[digit], expander);
    
    return digit;
}

void clear_display(uint8_t expander) {
    expander_write(0xFF, expander);   // all segments OFF
}
