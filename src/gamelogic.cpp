#include "gamelogic.h"
#include "nunchuckdraw.h"
#include "display.h"
#include "sendcommand.h"
#include "micros_timer.h"
#include "rc5decoder.h"

bool await_ack = false;
uint32_t await_time = 0;
uint8_t attempt_counter = 0;

void handle_place_boat(uint16_t selected_cell) {
    if (nunchuck_place_boat() && !await_ack) {
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
