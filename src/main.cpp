
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
#include "7segment.h"
#include "touch.h"
#include "touch.h"
#include <Arduino.h>
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
    // brightness_init();
    init_ir_sender();
    init_ir_receiver();
    nunchuck_init();
    grid_init();
    initCells(own_grid);
    USART_Init();    
    sei();
    uint8_t started = 0;
    uint8_t timer = 30;

    while(1){
    switch (currentGameState)
    {
    case STATE_START:{
        Startscreen_init();
        while(!started){
            if(screen_touched() && !started){
                grid_init();
                delay(50);
                dropBomb(3);
                _delay_ms(3000);
                dropBomb(10);
                _delay_ms(3000);
                started = 1;
            }
        }

        
        currentGameState=STATE_PLACE_BOATS;
        return;
    }
        

    case STATE_PLACE_BOATS:
        // Code to handle boat placement
        update_grid();
        drawRadarGrid();
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
        uint16_t selected_cell = joystick_select();

        handle_place_boat(selected_cell);
        update_grid();
        handle_ack(selected_cell);
        handle_ir_frame(selected_cell);
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
    // decode_ir();
    }
    }


