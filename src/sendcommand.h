#ifndef IR_SENDER_H
#define IR_SENDER_H

#include <stdint.h>

// Initialize the IR sender hardware
void init_ir_sender(void);

// Send an RC5 command
void send_command(uint8_t field, uint8_t address, uint8_t command);

#endif 
