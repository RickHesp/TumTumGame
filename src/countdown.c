#include <avr/io.h>
#include <util/delay.h>

#define EXPANDER_ADDR 0x21  // I2C expander address

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


void TWI_init(void) {
    TWSR = 0x00;     // prescaler = 1
    TWBR = 72;       // ~100kHz at 16MHz
}

void TWI_start(void) {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT))); //wait for the start-signal to be fully sent
}

void TWI_stop(void) {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void TWI_write(uint8_t data) {
    TWDR = data; //contains next bit to be sent
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

void expander_write(uint8_t data) {
    TWI_start();
    TWI_write(EXPANDER_ADDR << 1); //where to send the bits
    TWI_write(data);
    TWI_stop();
}

int countDown_start(void) {
    TWI_init();

    while (1) {
        for (int i = 0; i < 10; i++) {
            expander_write(digit_map[i]);
            _delay_ms(500);
        }
    }      
}