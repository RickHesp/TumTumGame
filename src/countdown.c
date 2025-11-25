#include <avr/io.h>
#include <util/delay.h>

#define EXPANDER_ADDR 0x20

const uint8_t digit_map[10] = {
    0x3F,
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D,
    0x7D,
    0x07,
    0x7F,
    0x6F
};

// --- TWI (I2C) Functions ---
void TWI_init(void) {
    TWSR = 0x00; // prescaler = 1
    TWBR = 72;   // ~100kHz @ 16MHz
}

void TWI_start(void) {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

void TWI_stop(void) {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void TWI_write(uint8_t data) {
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

// --- Send byte to port expander ---
void expander_write(uint8_t data) {
    TWI_start();
    TWI_write(EXPANDER_ADDR << 1); // SLA+W
    TWI_write(data);
    TWI_stop();
}

// --- Main program ---
int main(void) {
    TWI_init();

    while (1) {
        for (int i = 9; i >= 0; i--) {
            expander_write(digit_map[i]); // show digit
            _delay_ms(1000);              // wait 1s
        }
    }
}