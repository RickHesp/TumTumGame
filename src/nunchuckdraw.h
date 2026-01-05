#ifndef NUNCHUCKDRAW_H
#define NUNCHUCKDRAW_H

#include <stdint.h>
#include "nunchuck.h"
#include <Adafruit_ILI9341.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t move_joysticks(NunchuckJoystick_t joy);
uint8_t joystick_select();
uint8_t* joystick_select_boat(uint8_t boat_direction);
uint8_t nunchuck_c_button();
bool nunchuck_z_button();

#ifdef __cplusplus
}
#endif

#endif