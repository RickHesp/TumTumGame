
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
        uint16_t selected_cell = joystick_select();

        handle_place_boat(selected_cell);
        update_grid();
        handle_ack(selected_cell);
        handle_ir_frame(selected_cell);
    }
}
