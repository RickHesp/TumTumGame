#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <usart.h>
#include "sendcommand.h"
#include "irreceiver.h"
#include "brightness.h"

int main(void){
    USART_Init();
    USART_Print("IR sender/receiver ready\r\n");
    brightness_init();
    init_carrier();
    init_sender();
    timer1_init();
    init_receiver();
    sei();

    char halfbits[32];
    uint8_t halfcount = 0;

    while(1){
        if(send_next_command_flag){
            send_next_command_flag = 0;
            send_command(0, 1, 0x65);
        }

        uint16_t delta;
        uint8_t state;
        while(buffer_get(&delta, &state)){
            if(delta > 6000){
                for(uint8_t i=0; i+1<halfcount; i+=2){
                    if(halfbits[i]=='0' && halfbits[i+1]=='1') USART_putc('0');
                    else if(halfbits[i]=='1' && halfbits[i+1]=='0') USART_putc('1');
                    else USART_putc('?');
                }
                if(halfcount%2==1) USART_putc(halfbits[halfcount-1]);
                USART_putc('\n');
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
