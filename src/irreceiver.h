#ifndef IR_RECEIVER_H
#define IR_RECEIVER_H

#include <stdint.h>

#define HALFBIT_US 889
#define IR_PIN PD2
#define BUFFER_SIZE 64

typedef struct {
    uint16_t delta;
    uint8_t state; // 0 = LOW, 1 = HIGH
} delta_entry_t;

// Initialize hardware and buffers for IR reception
void init_ir_receiver(void);

// Retrieve a delta from the buffer
int buffer_get(uint16_t *val, uint8_t *state);

#endif