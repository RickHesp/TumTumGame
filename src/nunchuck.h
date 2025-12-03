#include "TWI.h"
#include <avr/io.h>
#include <util/delay.h>

typedef struct {
    uint8_t x;
    uint8_t y;
} NunchuckJoystick;

void nunchuck_init(uint8_t expander);
void nunchuck_read(uint8_t expander, uint8_t *buf);
NunchuckJoystick nunchuck_readJoystick(uint8_t expander);

void nunchuck_init(uint8_t expander){
    TWI_start();
    TWI_write(expander<<1); //open nunchuck in write mode
    TWI_write(0x40); //handshake (open)
    TWI_write(0x0); //no encryption
    _delay_ms(2);
}

void nunchuck_read(uint8_t expander, uint8_t *buf){
    TWI_write(expander<<1);
    TWI_write(0x0); //no encryption
    TWI_stop();

    _delay_ms(5);

    TWI_start();
    TWI_write(expander << 1);
    for(int i = 0; i<5; i++){
        buf[i] = TWI_read_ACK();
    }
    buf[5] = TWI_read_NACK();
    TWI_stop();
}

NunchuckJoystick nunchuck_readJoystick(uint8_t expander) {
    uint8_t buf[6];
    NunchuckJoystick joy;

    // Request new data
    TWI_start();
    TWI_write(expander << 1);
    TWI_write(0x00);
    TWI_stop();

    _delay_ms(5);


    TWI_start();
    TWI_write((expander << 1) | 1); // read mode
    for (int i = 0; i < 5; i++) {
        buf[i] = TWI_read_ACK();
    }
    buf[5] = TWI_read_NACK();
    TWI_stop();

    // Extract joystick values
    joy.x = buf[0];
    joy.y = buf[1];

    return joy;
}