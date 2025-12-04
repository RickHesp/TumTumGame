#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "nunchuck.h"

static int start_index = 1; //cursor position

int move_joysticks(NunchuckJoystick_t joy, int grid_size) {
    //difference in x,y
    int dx = 0;
    int dy = 0;

//define movements
    if (joy.x < 100) {
        dx = -1; // Move left
    } else if (joy.x > 155) {
        dx = 1;  // Move right
    }else if (joy.y > 175) {
        dy = -1; // Move up
    } else if (joy.y < 90) {
        dy = 1;  // Move down
    }
    
    uint8_t row = start_index / grid_size;
    uint8_t col = start_index % grid_size;

    //update
    uint8_t newRow = row + dy;
    uint8_t newCol = col + dx;

    start_index = newRow * grid_size + newCol + 1;


    return start_index;

}

#ifdef __cplusplus
}
#endif