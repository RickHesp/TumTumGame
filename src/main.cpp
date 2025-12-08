#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <usart.h>
#include <twi.h>
#include "sendcommand.h"
#include "irreceiver.h"
#include "brightness.h"
#include "drawGrid.h"
#include "rc5decoder.h"
#include "nunchuck.h"
#include "nunchuckdraw.h"

#define EXPANDER_ADDRESS 0x52

volatile uint16_t nunchuck_timer = 0;
uint16_t movetime = 100; //time between moves in ms

int main(void){
    init();//from arduino.h
    initCells();
    grid_init();
    brightness_init();
    init_ir_sender();
    init_ir_receiver();
    nunchuck_init(EXPANDER_ADDRESS);
    NunchuckJoystick_t joy;
    int index;

    USART_Init();
    USART_Print("IR sender/receiver ready\r\n");
    
    sei();

    char halfbits[32];
    uint8_t halfcount = 0;

    while(1){
        if(send_next_command_flag){
            send_next_command_flag = 0;
            send_command(1, 1, 5);
        }
        if (nunchuck_timer >= movetime) {
          joy = nunchuck_readJoystick(EXPANDER_ADDRESS);   
          index = move_joysticks(joy, 6);
          fill_cell(index, ILI9341_RED);
          nunchuck_timer = 0;
        }

        uint16_t delta;
        uint8_t state;
        while(buffer_get(&delta, &state)){
            if(delta > 6000){
                // Decode and store the frame
                rc5_frame_t received_frame = decode_rc5(halfbits, halfcount);

                if(received_frame.valid){
                    selectCell(received_frame.command);      
                    fill_grid();                   
                } else {
                    USART_Print("Invalid frame\n");
                }
                
                halfcount = 0;
                continue;
            }

            uint8_t count = 0;
            if(delta < 2400) count = 1;
            else if(delta > 2800) count = 2;

            for(uint8_t i=0; i<count && halfcount<32; i++)
                halfbits[halfcount++] = state ? '0' : '1';
        }
    }
}