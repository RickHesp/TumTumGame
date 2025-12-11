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

#define SEGMENT_ADDRESS 0x21
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
    TWI_init();

    USART_Init();
    USART_Print("IR sender/receiver ready\r\n");
    
    uint8_t count = 10;
    uint8_t timer = 15;
    clear_display(SEGMENT_ADDRESS);

    sei();

    while(1){
        if(one_second_passed()){
            if(timer >= 10){
                timer--;
            }else if(timer < 10 && timer > 0){
                countDown_step(SEGMENT_ADDRESS, count);
                count--;
                timer--;
            }
            else if (timer == 0){
                countDown_step(SEGMENT_ADDRESS, 8);
                timer = 15;
                count = 10;
            }
        }
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