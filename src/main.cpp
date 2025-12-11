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

#define EXPANDER_ADRESS 0x52
uint16_t lastmove = 0;
uint8_t boatindex = 0;

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
        uint16_t selected_cell = joystick_select();
        boatindex = nunchuck_place_boat();
        if(boatindex > 0){
            send_command(1, 1, selected_cell);

            // await confirmation
            // placeBoat(index);
        }
        if(micros_timer() - lastmove > 100){
            fill_grid(own_grid);
            lastmove = micros_timer();

        }
    rc5_frame_t frame = decode_ir();  
    if(frame.valid){
        if(frame.command == 63){
            placeBoat(boatindex);

        }
        else{
            send_command(1, 1, 63);
        }


        // if it is player sender turn
        // send back confirmation
        // selectCell(received_frame.command);      
        // else
        // if player has received message before
        // send back confirmation
        // else
        // error

        // send confirmation:
    }
    USART_putc('0' + frame.valid); 
    }
}