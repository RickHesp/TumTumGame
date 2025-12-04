#ifndef NUNCHUCK_H
#define NUNCHUCK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t x;
    uint8_t y;
    bool* cButton;
    bool* zButton;
} NunchuckJoystick_t;

void nunchuck_init(uint8_t expander);
void nunchuck_read(uint8_t expander, uint8_t *buf);
NunchuckJoystick_t nunchuck_readJoystick(uint8_t expander);
void read_buttons(uint8_t expander, bool* cButton, bool* zButton);

#ifdef __cplusplus
}
#endif

#endif // NUNCHUCK_H