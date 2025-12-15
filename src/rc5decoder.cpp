#include <stdint.h>
#include <usart.h>
#include <stdio.h>
#include "rc5decoder.h"
#include "display.h"
#include "irreceiver.h"
#include "grid.h"
#include "sendcommand.h"

#define ACK_ADDRESS 31
    char halfbits[32];
    uint8_t halfcount = 0;


rc5_frame_t decode_rc5(char* halfbits, uint8_t halfcount) {
    rc5_frame_t frame = {0};
     
    // Skip leading invalid pairs (00 or 11)
    uint8_t start_offset = 0;
    while (start_offset + 1 < halfcount) {
        if((halfbits[start_offset] == '0' && halfbits[start_offset+1] == '0') ||
           (halfbits[start_offset] == '1' && halfbits[start_offset+1] == '1')) {

            start_offset += 2;
        } else {
            break;
        }
    }
    
    // Fix odd half-bit count
    uint8_t adjusted_count = halfcount - start_offset;
    if (adjusted_count % 2 == 1) {
        halfbits[halfcount] = (halfbits[halfcount-1] == '0') ? '1' : '0';
        halfcount++;
        adjusted_count++;
    }
    
    // Decode Manchester pairs
    uint16_t decoded = 0;
    uint8_t bit_count = 0;
    
    for (uint8_t i = start_offset; i + 1 < halfcount && bit_count < 14; i += 2) {
        if (halfbits[i] == '0' && halfbits[i+1] == '1') {
            bit_count++;
        } 
        else if (halfbits[i] == '1' && halfbits[i+1] == '0') {
            decoded |= (1 << (13 - bit_count));
            bit_count++;
        } 
        else {
            USART_Print("\nInvalid Manchester pair\n");
            frame.valid = 0;
            return frame;
        }
    }

    frame.start_bit = (decoded >> 13) & 1;
    frame.field_bit = (decoded >> 12) & 1;
    frame.toggle_bit = (decoded >> 11) & 1;
    frame.address   = (decoded >> 6)  & 0x1F;

    uint8_t cmd_lower = decoded & 0x3F;
    uint8_t cmd_bit6  = (frame.field_bit == 0) ? 1 : 0;
    frame.command     = cmd_lower | (cmd_bit6 << 6);

    // Mark as valid if the command was decoded.
    frame.valid = 1;

    return frame;
}

void decode_ir(){
    uint16_t delta;
    uint8_t state;
    while(buffer_get(&delta, &state)){
    
    if(delta > 5000){      
        // Decode and store the frame
        rc5_frame_t received_frame = decode_rc5(halfbits, halfcount);
        if(received_frame.address == ACK_ADDRESS && received_frame.valid){
            receive_acknowledgement();
        } else if(received_frame.valid){
            send_ack();
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
