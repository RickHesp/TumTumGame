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


int main(void){
    init();//from arduino.h
    brightness_init();
    init_ir_sender();
    init_ir_receiver();

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

        uint16_t delta;
        uint8_t state;
        while(buffer_get(&delta, &state)){
            if(delta > 6000){
                // Decode and store the frame
                rc5_frame_t received_frame = decode_rc5(halfbits, halfcount);

                if(received_frame.valid){
                    selectCell(received_frame.command);      
                    //fill_grid();                   
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