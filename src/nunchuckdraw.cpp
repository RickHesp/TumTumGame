#include "nunchuckdraw.h"


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
    if(joy.x == 128 && joy.y == 128){
        return start_index; //no movement
    }else{
         if (joy.x < 100) {
            dx = -1; // Move left
        } else if (joy.x > 155) {
            dx = 1;  // Move right
        }else if (joy.y > 175) {
            dy = -1; // Move up
        } else if (joy.y < 90) {
            dy = 1;  // Move down
    }
    }
   
    
    int row = start_index / grid_size;
    int col = start_index % grid_size;

    int newRow = row + dy;
    int newCol = col + dx;

    // clamp boundaries
    if (newRow < 0) newRow = 0;
    if (newRow >= grid_size) newRow = grid_size - 1;
    if (newCol < 0) newCol = 0;
    if (newCol >= grid_size) newCol = grid_size - 1;

    color_cell(start_index, ILI9341_BLUE);

    start_index = newRow * grid_size + newCol + 1;


    if(start_index < 1 || start_index > grid_size * grid_size){
        start_index -= (dy * grid_size + dx);
    }else if(newCol < 0 || newCol >= grid_size){
        start_index -= (dy * grid_size + dx);
    }


    return start_index;

}

#ifdef __cplusplus
}
#endif