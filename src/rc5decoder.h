#ifndef RC5DECODER_H
#define RC5DECODER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t start_bit;
    uint8_t field_bit;
    uint8_t toggle_bit;
    uint8_t address;
    uint8_t command;
    uint8_t valid;
} rc5_frame_t;

// Decodes halfbit stream into an RC5 frame
rc5_frame_t decode_rc5(char* halfbits, uint8_t halfcount);

// Pulls timings from buffer_get() and decodes complete frames
void decode_ir(void);

#ifdef __cplusplus
}
#endif
#endif
