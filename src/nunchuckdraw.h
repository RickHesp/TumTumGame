#ifndef NUNCHUCKDRAW_H
#define NUNCHUCKDRAW_H

#include <stdint.h>
#include "nunchuck.h"
#include <Adafruit_ILI9341.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t move_joysticks(NunchuckJoystick_t joy);
void joystick_select();
void nunchuck_place_boat();

#ifdef __cplusplus
}
#endif

#endif