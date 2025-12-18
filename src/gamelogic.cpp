#include "gamelogic.h"
#include "nunchuckdraw.h"
#include "display.h"
#include "sendcommand.h"
#include "micros_timer.h"
#include "rc5decoder.h"
#include "grid.h"

bool await_ack = false;
uint32_t await_time = 0;
uint8_t attempt_counter = 0;

void handle_place_boat(uint16_t selected_cell) {
    if (nunchuck_z_button() && !await_ack) {
        send_command(1, CMD_PLACE_BOAT, selected_cell);
        await_ack = true;
        await_time = micros_timer();
        attempt_counter = 0;
    }
}

void handle_ack(uint16_t selected_cell) {
    if (await_ack && (micros_timer() - await_time > ACK_TIMEOUT)) {
        send_command(1, CMD_RETRY, selected_cell);
        attempt_counter++;
        if (attempt_counter > MAX_ATTEMPTS) await_ack = false;
    }
}

void handle_ir_frame(uint16_t selected_cell) {
    rc5_frame_t frame = decode_ir();
    if (!frame.valid) return;

    if (frame.address == ADDR_ACK && frame.command == selected_cell) {
        hitCell(frame.command);
        await_ack = false;
    } else if (frame.address != ADDR_ACK) {
        send_command(1, ADDR_ACK, frame.command);
        placeBoat(frame.command);
    }
}

bool boat_placement(gridCell *grid){
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
        }
        placed_boats++;
    }
    return false;
}
