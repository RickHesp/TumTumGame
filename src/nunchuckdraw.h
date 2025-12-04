#include "nunchuck.h"

#ifdef __cplusplus
extern "C" {
#endif

void draw_init();
int move_joysticks(NunchuckJoystick_t joy, int grid_size);
void updateCube(uint8_t expander);

#ifdef __cplusplus
}
#endif
