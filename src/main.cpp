
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
#include "7segment.h"

#include "touch.h"
#include <Arduino.h>
#include "micros_timer.h"
#include "gamelogic.h"

bool startingPlayer = false; //true if this device starts first
GameState currentGameState = STATE_START;

int main(void){
    init();//from arduino.h
    // brightness_init();
    init_ir_sender();
    init_ir_receiver();
    nunchuck_init();
    initCells(own_grid);
    Startscreen_init();
    USART_Init();    

    sei();
    uint8_t started = 0;
    uint8_t timer = 30;

    while(1){

    switch (currentGameState)
    {
    case STATE_START:
        if(ir_start_command_received()){
            started = 1;
            startingPlayer = false;
            grid_init();
            currentGameState=STATE_PLACE_BOATS;
        }

        if(screen_touched() && !started){
        started = 1;
        grid_init();
        currentGameState=STATE_PLACE_BOATS;
    }
        break;
    
case STATE_PLACE_BOATS:
    // Use nunchuck to select a head+tail and simulate drawing the boat when Z is pressed
    update_grid();
    {
        uint8_t *inds = joystick_select_boat(nunchuck_c_button()); // returns [head, tail]
        if (screen_touched()){
            // place both cells and draw the ship spanning the two selected cells
            placeBoat(inds[0]);
            placeBoat(inds[1]);
            drawBoat(inds[0], inds[1]);
            // advance to setup (adjust as needed for multiple boats)
            currentGameState = STATE_SETUP_GAME;
        }
    }
     break;

    case STATE_SETUP_GAME:
        // Code to handle game setup

        initCells(opp_grid);
        timer_init(timer);
        fill_grid(opp_grid);
        if (startingPlayer) {
            currentGameState = STATE_YOUR_TURN;
        } else {
            currentGameState = STATE_OPPONENT_TURN;
        }        break;
    case STATE_YOUR_TURN:
        drawYourTurn(1);

        shoot_salvo(opp_grid);
        update_opp_grid(); // State = opp turn
        handle_ack();
        handle_ir_frame();
            if(one_second_passed() && started && timer > 0){
            timer--; // decrease timer once every second
            timer_init(timer);
            }

            if(timer == 0){
                currentGameState=STATE_OPPONENT_TURN;
            }   

        break;

    case STATE_OPPONENT_TURN:
        //Code to handle opponent's turn
        drawYourTurn(0);
        update_grid();
        handle_ack();
        handle_ir_frame(); // State = your turn

        break;

    case STATE_GAME_OVER:
        //Code to handle game over
        break;
    default:
        break;
    }
    }
}

