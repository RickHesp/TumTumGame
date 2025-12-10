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

    char halfbits[32];
    uint8_t halfcount = 0;

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

    uint16_t delta;
    uint8_t state;
    while(buffer_get(&delta, &state)){
    
    if(delta > 5000){      
        // Decode and store the frame
        rc5_frame_t received_frame = decode_rc5(halfbits, halfcount);
        
        if(received_frame.valid){
            selectCell(received_frame.command);      
            USART_putc('0' + (received_frame.command / 10));
            USART_putc('0' + (received_frame.command % 10));
            USART_putc('\n');
        } else {
            USART_Print("Invalid frame\n");
        }
        
        halfcount = 0;
        continue;
    }

    // Ignore very short pulses
    if(delta < 800) continue;

    // Count halfbits based on timing
    uint8_t count = 0;
    if(delta >= 1400 && delta <= 2600) {
        count = 1;
    } else if(delta >= 2900 && delta <= 4200) {
        count = 2;
    }

    // Add halfbits to buffer
    for(uint8_t i=0; i<count && halfcount<32; i++)
        halfbits[halfcount++] = state ? '0' : '1';
}
}
}