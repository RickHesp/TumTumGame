#include "gamelogic.h"
#include "nunchuckdraw.h"
#include "display.h"
#include "sendcommand.h"
#include "micros_timer.h"
#include "rc5decoder.h"
#include "grid.h"
#include <usart.h>

bool await_ack = false;
bool await_ack_switch = false;
uint32_t await_time = 0;
uint8_t attempt_counter = 0;
uint16_t pending_cell = 0;

void handle_ack() {
    if (await_ack && (micros_timer() - await_time > ACK_TIMEOUT)) {
        send_command(1, ADDR_SHOOT, pending_cell);
        await_time = micros_timer();
        attempt_counter++;
        if (attempt_counter > MAX_ATTEMPTS){
            await_ack = false;
            // show error: no connection
        }
    }
}

void handle_ack_switch(){
    if(await_ack_switch && (micros_timer() - await_time > ACK_TIMEOUT)){
        send_command(1, ADDR_SWITCH_PLAYER, 1);
        await_time = micros_timer();
        attempt_counter++;
        if(attempt_counter > MAX_ATTEMPTS){
            await_ack_switch = false;
            // show error: no connection
        }
    }
}

void handle_ir_frame() {
    rc5_frame_t frame = decode_ir();
    if (!frame.valid) return;

    if (frame.address == ADDR_ACK && await_ack && frame.command == pending_cell) {
        oppHitCell(frame.command);
        await_ack = false;
        currentGameState=STATE_OPPONENT_TURN;
    } else if (frame.address == ADDR_SHOOT) {
        send_command(1, ADDR_ACK, frame.command);
        hitCell(frame.command);
        currentGameState=STATE_YOUR_TURN;
    } else if (frame.address == ADDR_SWITCH_PLAYER){
        send_command(1, ADDR_ACK_SWITCH, 1);
        currentGameState=STATE_YOUR_TURN;
    } else if (frame.address == ADDR_ACK_SWITCH){
        currentGameState=STATE_OPPONENT_TURN;
    }
    
}

bool ir_start_command_received(){
    rc5_frame_t frame = decode_ir();
    if (!frame.valid) return false;
    if(frame.address == 2) return true;
    return false;
}

bool boat_placement(gridCell* grid){
    // Code to handle boat placement
    static uint8_t placed_boats = 0;

    uint8_t* index_array = joystick_select_boat(nunchuck_c_button());
    if(nunchuck_z_button()){
        //check if 4 boats have been placed
        if(placed_boats > 3){
            return true; //all boats placed
        }
        //check if no boat is present at selected cells
        for(uint8_t i=0; i<sizeof(index_array)/sizeof(index_array[0]); i++){
            if(grid[index_array[i]].boat == 1) return false; //boat already present, do not place
        }
        //if no boats are present, place boat cells
        for(uint8_t i=0; i<sizeof(index_array)/sizeof(index_array[0]); i++){
            placeBoat(index_array[i]);//place boat at selected cell
            drawBoat(index_array[i], index_array[i+1]);//draw boat on display
        }
        placed_boats++;
    }
    return false;
}
void shoot_salvo(gridCell *grid){
    uint8_t selected_cell = opp_joystick_select();

    if(grid[selected_cell].hit == 1) return;

    if (nunchuck_z_button() && !await_ack) {
        send_command(1, ADDR_SHOOT, selected_cell);
        await_ack = true;
        await_time = micros_timer();
        attempt_counter = 0;
        pending_cell = selected_cell;
    }
}
