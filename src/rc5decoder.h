#ifndef RC5DECODER_H
#define RC5DECODER_H

#include <stdint.h>
#include <stdbool.h>

// RC5 constants
#define MAX_HALFBITS 32
#define RC5_TOTAL_BITS 14
#define RC5_ADDRESS_BITS 5
#define RC5_COMMAND_BITS 6

// RC5 frame structure
typedef struct {
    uint8_t start_bit;
    uint8_t field_bit;
    uint8_t toggle_bit;
    uint8_t address;
    uint8_t command;
    uint8_t valid;
} rc5_frame_t;

rc5_frame_t decode_rc5(char* halfbits, uint8_t halfcount);
rc5_frame_t decode_ir(void);

#endif // RC5DECODER_H