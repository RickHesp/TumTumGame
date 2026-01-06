
#include "nunchuckdraw.h"
#include "nunchuck.h"
#include "micros_timer.h"
#include "display.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CENTER_X 128
#define CENTER_Y 128
#define MARGIN 5
#define MOVE_DELAY 1//in microseconds
#define DIR_RIGHT 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_UP 3

static uint8_t start_index = 0; //cursor position
NunchuckJoystick_t joy;
//selected cell index
uint8_t index=0;//default starting position
uint8_t old_index=0;

//tail index for boats
uint8_t boat_tail_index=0;
uint8_t new_tail_index=0;
uint8_t old_tail_index=0;


uint32_t last_time = 0;//time of last movement

uint8_t new_tail=0;

static inline bool is_in_bounds(int row, int col) {
    return (row >= 0 && row < GRID_HEIGHT && col >= 0 && col < GRID_WIDTH);
}

uint8_t move_joysticks(NunchuckJoystick_t joy) {
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
    int col = start_index % GRID_WIDTH;

int next_row = row + dy;
int next_col = col + dx;

//only move if inbound, otherwise ignore input
if (is_in_bounds(next_row, next_col)) {
    old_index = start_index;
    start_index = next_row * GRID_WIDTH + next_col;
}

return start_index; 
}

//function for the selecting of a single cell in the grid
uint8_t joystick_select() {
    uint32_t current = micros_timer();
    if(current - last_time > MOVE_DELAY){
        joy = nunchuck_readJoystick();
        index = move_joysticks(joy);
        selectCell(index);
        if(old_index != index) deselectCell(old_index);
        last_time = current;
    }
    return index;
}

//compute tail index from head+direction (tail is "behind" the head)
bool boat_tail_from_head(uint16_t head_index, uint8_t boat_direction) {
    int head_row = head_index / GRID_WIDTH;
    int head_col = head_index % GRID_WIDTH;

    int tail_row = head_row;
    int tail_col = head_col;

    switch (boat_direction)
    {
    case DIR_RIGHT: tail_col = head_col - 1;
        break;
    case DIR_DOWN: tail_row = head_row + 1;
        break;
    case DIR_LEFT: tail_col = head_col + 1;
        break;
    case DIR_UP: tail_row = head_row - 1;
        break;
    default:
        return false;
    }
    
    if (!is_in_bounds(tail_row, tail_col)) return false;

    new_tail_index = (tail_row * GRID_WIDTH + tail_col);
    return true;
}

//function to place a boat tail based on the head index and a direction value
uint8_t* joystick_select_boat(uint8_t boat_direction) {
    uint32_t current = micros_timer();
    if (current - last_time > MOVE_DELAY) {

        joy = nunchuck_readJoystick();

        // Save old head+tail before attempting move
        old_index = start_index;
        old_tail_index = boat_tail_index;

        //figure out head index
        index = move_joysticks(joy);

        //looks if the new tail location is valid, if not, it ignores the input
        if (!boat_tail_from_head(index, boat_direction)) {
            //revert head index
            start_index = old_index;
            index = old_index;
            boat_tail_index = old_tail_index;
        } else {
            boat_tail_index = new_tail_index;
        }

        //save the index locations
        selectCell(index);
        selectCell(boat_tail_index);

        //delete old cells if changed
        if (old_index != index) deselectCell(old_index);
        if (old_tail_index != boat_tail_index) deselectCell(old_tail_index);

        last_time = current;
    }
    static uint8_t index_array[2];
    index_array[0] = index;
    index_array[1] = boat_tail_index;
    return index_array;
}

//function for getting playerinput for the boat direction
uint8_t nunchuck_c_button(){
    static uint8_t direction = 0;
    if (direction > 3) direction = 0;

    if(joy.cButton){
        //return a number between 0-3 for boat direction
        return direction++;
    }
    return direction;
}

bool nunchuck_z_button(){
    if(joy.zButton){
        return true;
    }
    return false;
}
#ifdef __cplusplus
}
#endif
