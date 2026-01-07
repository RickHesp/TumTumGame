#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdint.h>
#include "grid.h"

#define CMD_PLACE_BOAT 1
#define CMD_RETRY 2
#define ADDR_ACK 21
#define ACK_TIMEOUT 500000UL
#define MAX_ATTEMPTS 20

enum GameState {
    STATE_START,
    STATE_PLACE_BOATS,
    STATE_SETUP_GAME,
    STATE_YOUR_TURN,
    STATE_OPPONENT_TURN,
    STATE_GAME_OVER
};

extern bool await_ack;
extern uint32_t await_time;
extern uint8_t attempt_counter;
extern GameState currentGameState;

void handle_ack();
void handle_ir_frame();
bool ir_start_command_received();
bool boat_placement(gridCell *grid);
void shoot_salvo(gridCell *grid);
#endif
