
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <usart.h>
#include <Arduino.h>
#include "sendcommand.h"
#include "irreceiver.h"
#include "brightness.h"
#include "display.h"
#include "rc5decoder.h"
#include "nunchuck.h"
#include "nunchuckdraw.h"
#include "TWI.h"
#include "micros_timer.h"
#include "gamelogic.h"

enum GameState {
    STATE_START,
    STATE_PLACE_BOATS,
    STATE_SETUP_GAME,
    STATE_YOUR_TURN,
    STATE_OPPONENT_TURN,
    STATE_GAME_OVER
};

GameState currentGameState = STATE_START;

int main(void){
    init();//from arduino.h
    brightness_init();
    init_ir_sender();
    init_ir_receiver();
    nunchuck_init();
    grid_init();
    initCells(own_grid);
    USART_Init();    
    sei();
    while(1){
    switch (currentGameState)
    {
        case STATE_START:
            currentGameState=STATE_PLACE_BOATS;

            //plaats hier een bool voor de startknop
            if(true){
                grid_init();
                initCells(own_grid);
                currentGameState=STATE_PLACE_BOATS;
            }
            break;

        case STATE_PLACE_BOATS:
            // Code to handle boat placement
            update_grid();
            if(boat_placement(own_grid)){
                currentGameState=STATE_SETUP_GAME;
            }
            break;

        case STATE_SETUP_GAME:
            // Code to handle game setup
            grid_init();
            initCells(opp_grid);
            
            fill_grid(opp_grid);
            currentGameState=STATE_YOUR_TURN;
            break;

        case STATE_YOUR_TURN:
            shoot_salvo(opp_grid);
            update_opp_grid();
            handle_ack();
            handle_ir_frame();
            break;

        case STATE_OPPONENT_TURN:
            //Code to handle opponent's turn
            break;

        case STATE_GAME_OVER:
            //Code to handle game over
            break;

        default:
            break;
        }
    }
}

