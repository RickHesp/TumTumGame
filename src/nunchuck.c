#include "countdown.h"

#define EXPANDER_ADDR 0x21

void nunchuck_init(uint8_t expander){
    TWI_start();
    TWI_write(expander<<1);
    TWI_write(0x40); //handshake (open)
    TWI_write(0x0); //no encryption

}

void nunchuck_read(uint8_t expander){
    TWI_write(expander<<1);
    TWI_write(0x0); //no encryption
}