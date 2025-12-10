#include "TWI.h"
#include <avr/io.h>
#include <util/delay.h>
#include "nunchuckdraw.h"
#include "nunchuck.h"

#define EXPANDER_ADRESS 0x52

void nunchuck_init(){
    TWI_start();
    TWI_write(EXPANDER_ADRESS<<1); //open nunchuck in write mode
    TWI_write(0x40); //handshake (open)
    TWI_write(0x0); //no encryption
    _delay_ms(2);
}

void nunchuck_read(uint8_t *buf){
    TWI_start();
    TWI_write(EXPANDER_ADRESS<<1);
    TWI_write(0x0); //no encryption
    TWI_stop();

    _delay_ms(5);

    TWI_start();
    TWI_write(EXPANDER_ADRESS << 1|1);
    for(int i = 0; i<5; i++){
        buf[i] = TWI_read_ACK();
    }
    buf[5] = TWI_read_NACK();
    TWI_stop();
}

NunchuckJoystick_t nunchuck_readJoystick() {
    uint8_t buf[6];
    NunchuckJoystick_t joy;

    // Request new data
    TWI_start();
    TWI_write(EXPANDER_ADRESS << 1);
    TWI_write(0x00);
    TWI_stop();

    _delay_ms(1);


    TWI_start();
    TWI_write((EXPANDER_ADRESS << 1) | 1); // read mode
    for (int i = 0; i < 5; i++) {
        buf[i] = TWI_read_ACK();
    }
    buf[5] = TWI_read_NACK();
    TWI_stop();

    // Extract joystick values
    joy.x = buf[0];
    joy.y = buf[1];
    joy.zButton = !(buf[5] & 0x01); // bit 1
    joy.cButton = !((buf[5] >> 1) & 0x01); // bit 0
    return joy;
}

void read_buttons(bool *cButton, bool *zButton) {
    uint8_t buf[6];
    nunchuck_read(buf);

    // get button states
    *zButton = !(buf[5] & 0x01);  // bit 0
    *cButton = !((buf[5] >> 1) & 0x01); //bit 1

}