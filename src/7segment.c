#include <avr/io.h>
#include <util/delay.h>
#include "TWI.h"

const uint8_t digit_map[10] = {
    0b11000000, // 0
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
    TWI_write(expander << 1); //where to send the bits
    TWI_write(data);
    TWI_stop();
}

int countDown_start(uint8_t expander) {
    TWI_init();

    while (1) {
      for (int i = 0; i < 10; i++) {
            expander_write(digit_map[i], expander);
            _delay_ms(500);
        }
    }      
}

int countDown_step(uint8_t expander, int digit) {
    expander_write(digit_map[digit], expander);
    return digit;
}