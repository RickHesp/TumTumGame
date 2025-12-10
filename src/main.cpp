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
        if(nunchuck_place_boat()){
            USART_Print("Z");
            send_command(1, 1, selected_cell);
        }
        if(micros_timer() - lastmove > 100){
            fill_grid(own_grid);
            lastmove = micros_timer();

        }
    decode_ir();   
    }
}