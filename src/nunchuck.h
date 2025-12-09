#ifndef NUNCHUCK_H
#define NUNCHUCK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t x;
    uint8_t y;
    bool cButton;
    bool zButton;
} NunchuckJoystick_t;

extern NunchuckJoystick_t joy;

void nunchuck_init( );
void nunchuck_read(uint8_t *buf);
NunchuckJoystick_t nunchuck_readJoystick();


#ifdef __cplusplus
}
#endif

#endif // NUNCHUCK_H