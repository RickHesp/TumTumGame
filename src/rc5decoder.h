#include <stdint.h>

typedef struct {
    uint8_t start_bit;
    uint8_t field_bit;
    uint8_t toggle_bit;
    uint8_t address;
    uint8_t command;
    uint8_t valid;
} rc5_frame_t;

rc5_frame_t decode_rc5(char* halfbits, uint8_t halfcount) {
    rc5_frame_t frame = {0};
    
    // Sometimes the last half bit is missing
    if (halfcount < 27) {
        frame.valid = 0;
        return frame;
    }
    
    // When the last half bit is missing, it is the inverse of half bit 27
    if (halfcount == 27) {
        halfbits[27] = (halfbits[26] == '0') ? '1' : '0';
        halfcount = 28;
    }
    
    // Decode Manchester encoding
    uint16_t decoded = 0;
    uint8_t bit_count = 0;
    
    for (uint8_t i = 0; i + 1 < halfcount && bit_count < 14; i += 2) {
        if (halfbits[i] == '0' && halfbits[i+1] == '1') {
            // 0
            bit_count++;
        } else if (halfbits[i] == '1' && halfbits[i+1] == '0') {
            // 1
            decoded |= (1 << (13 - bit_count));
            bit_count++;
        } else {
            // Invalid Manchester encoding
            frame.valid = 0;
            return frame;
        }
    }
    
    // Build struct
    frame.start_bit = (decoded >> 13) & 1;
    frame.field_bit = (decoded >> 12) & 1;
    frame.toggle_bit = (decoded >> 11) & 1;
    frame.address = (decoded >> 6) & 0x1F;
    frame.command = decoded & 0x3F;
    frame.valid = (bit_count == 14 && frame.start_bit == 1);
    
    return frame;
}