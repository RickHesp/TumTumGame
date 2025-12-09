#include "nunchuckdraw.h"
#include "nunchuck.h"
#include "micros_timer.h"
#include "display.h"


#ifdef __cplusplus
extern "C" {
#endif

#define CENTER_X 128
#define CENTER_Y 128
#define MARGIN 15
#define MOVE_DELAY 0

static int start_index = 0; //cursor position
NunchuckJoystick_t joy;
uint16_t index=0;//default starting position
uint16_t old_index=index;
uint32_t last_time = 0;

uint16_t move_joysticks(NunchuckJoystick_t joy) {
    //difference in x,y
    int dx = 0;
    int dy = 0;

    
    //define movements with a small margin with no response to joystick movement
    if(abs(joy.x - CENTER_X) < MARGIN && abs(joy.y - CENTER_Y) < MARGIN){
        return start_index; //no movement
    }
    //give priority to larger movement(as to not move diagonally)
    if(abs(joy.x - CENTER_X) >= abs(joy.y - CENTER_Y)){
        if (joy.x < CENTER_X) {
            dx = -1; // Move left
        } else if (joy.x > CENTER_X) {
            dx = 1;  // Move right
        }
    }else{
        if (joy.y > CENTER_Y) {
            dy = -1; // Move up
        } else if (joy.y < CENTER_Y) {
            dy = 1;  // Move down
        }
    }
   
    
    int row = start_index / GRID_WIDTH;
    int col = start_index % GRID_HEIGHT;

    if(dx!=0){
        col=(col + dx + GRID_WIDTH) % GRID_WIDTH;
    }
    if(dy!=0){
        row=(row + dy + GRID_HEIGHT) % GRID_HEIGHT;
    }

    old_index = start_index;
    start_index = row * GRID_WIDTH + col;

    return start_index;  
}

uint16_t joystick_select(){
    joy = nunchuck_readJoystick();
    index = move_joysticks(joy);
    selectCell(index);
    deselectCell(old_index);
    return index;
}

bool nunchuck_place_boat(){
    if(joy.zButton){
        placeBoat(index);
        return true;
    }
    return false;
}

#ifdef __cplusplus
}
#endif