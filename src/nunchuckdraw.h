#ifndef NUNCHUCKDRAW_H
#define NUNCHUCKDRAW_H

#include <stdint.h>
#include "nunchuck.h"
#include <Adafruit_ILI9341.h>

#ifdef __cplusplus
extern "C" {
#endif

int move_joysticks(NunchuckJoystick_t joy, int grid_size);
void color_cell(uint8_t cell, uint16_t color);

#ifdef __cplusplus
}
#endif

#endif