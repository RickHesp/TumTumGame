
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
bool await_ack = false;
uint32_t await_time = 0;
uint8_t attempt_counter = 0;
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
        if(nunchuck_place_boat() && !await_ack){
            send_command(1, 1, selected_cell);
            await_ack = true;
            await_time = micros_timer();
            attempt_counter = 0;
        }
        if(micros_timer() - lastmove > 100){
            fill_grid(own_grid);
            lastmove = micros_timer();
        }
        if(await_ack && (micros_timer() - await_time > 500000UL)){
            USART_Print("ERROR: no ack\n");
            send_command(1, 2, selected_cell);
            attempt_counter++;
            if(attempt_counter > 20) await_ack = false;
        }
        
        rc5_frame_t frame = decode_ir();   
        if(frame.valid){
            if(frame.address == 21){
                if(frame.command == selected_cell){
                    hitCell(frame.command);
                    await_ack = false;
                }
                
            }
            else{
                send_command(1, 21, frame.command);
                placeBoat(frame.command);
            }
        }
    }
}
