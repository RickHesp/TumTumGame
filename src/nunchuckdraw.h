#ifndef NUNCHUCKDRAW_H
#define NUNCHUCKDRAW_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void draw_init();
void draw_cursor(NunchuckJoystick_t joy);
void move_joystick(NunchuckJoystick_t joy);
void color_cell(int index, uint16_t color);
void drawCubeAtIndex(int index, uint16_t color);
void updateCube(uint8_t expander);

#ifdef __cplusplus
}
#endif

#endif // NUNCHUCK_H