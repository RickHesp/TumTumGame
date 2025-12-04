#include "nunchuck.h"

#ifdef __cplusplus
extern "C" {
#endif

void draw_init();
void draw_cursor(NunchuckJoystick_t joy);
int move_joystick(NunchuckJoystick_t joy);
void drawCubeAtIndex(int index, uint16_t color);
void updateCube(uint8_t expander);

#ifdef __cplusplus
}
#endif
