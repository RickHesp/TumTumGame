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
#include "game_state.h"

#define EXPANDER_ADRESS 0x52
uint16_t lastmove = 0;

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
    USART_Print("IR sender/receiver ready\r\n");
    
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
        if(micros_timer() - lastmove > 100){
            fill_grid(own_grid,1);
            lastmove = micros_timer();
        }
        if(boat_placement(own_grid)){
            //currentGameState=STATE_SETUP_GAME;
        }
        break;

    case STATE_SETUP_GAME:
        // Code to handle game setup
        grid_init();
        initCells(opp_grid);
        
        fill_grid(opp_grid, 1);
        currentGameState=STATE_PLACE_BOATS;
        break;

    case STATE_YOUR_TURN:

        fill_grid(opp_grid, 0);
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



        // uint8_t selected_cell = joystick_select();
        // if(nunchuck_z_button()){
        //     USART_Print("Z");
        //     send_command(1, 1, selected_cell);
        // }
        // if(micros_timer() - lastmove > 100){
        //     fill_grid(own_grid);
        //     lastmove = micros_timer();

        // }
    decode_ir();
    await_ack();   
    }
}
