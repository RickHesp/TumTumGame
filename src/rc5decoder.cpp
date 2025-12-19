#include "rc5decoder.h"
#include "irreceiver.h"

// Internal state
char halfbits[MAX_HALFBITS];
uint8_t halfcount = 0;

rc5_frame_t decode_rc5(char* halfbits, uint8_t halfcount) {
    rc5_frame_t frame = {0};

    // Skip leading invalid pairs (00 or 11)
    uint8_t start_offset = 0;
    while (start_offset + 1 < halfcount) {
        if ((halfbits[start_offset] == '0' && halfbits[start_offset+1] == '0') ||
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

    for (uint8_t i = start_offset; i + 1 < halfcount && bit_count < RC5_TOTAL_BITS; i += 2) {
        if (halfbits[i] == '0' && halfbits[i+1] == '1') {
            bit_count++;
        } else if (halfbits[i] == '1' && halfbits[i+1] == '0') {
            decoded |= (1 << (RC5_TOTAL_BITS - 1 - bit_count));
            bit_count++;
        } else {
            frame.valid = 0;
            return frame;
        }
    }

    frame.start_bit = (decoded >> 13) & 1;
    frame.field_bit = (decoded >> 12) & 1;
    frame.toggle_bit = (decoded >> 11) & 1;
    frame.address   = (decoded >> RC5_COMMAND_BITS) & ((1 << RC5_ADDRESS_BITS) - 1);

    uint8_t cmd_lower = decoded & ((1 << RC5_COMMAND_BITS) - 1);
    uint8_t cmd_bit6  = (frame.field_bit == 0) ? 1 : 0;
    frame.command     = cmd_lower | (cmd_bit6 << 6);

    frame.valid = 1;
    return frame;
}

rc5_frame_t decode_ir() {
    uint16_t delta;
    uint8_t state;
    rc5_frame_t received_frame = {0};

    while (buffer_get(&delta, &state)) {

        // End of frame
        if (delta > 5000) {      
            received_frame = decode_rc5(halfbits, halfcount);
            halfcount = 0;
            return received_frame;
        }

        // Ignore very short pulses
        if (delta < 800) continue;

        // Count halfbits based on timing
        uint8_t count = 0;
        if (delta >= 1400 && delta <= 2600) {
            count = 1;
        } else if (delta >= 2900 && delta <= 4200) {
            count = 2;
        }

        // Add halfbits to buffer
        for (uint8_t i = 0; i < count && halfcount < MAX_HALFBITS; i++)
            halfbits[halfcount++] = state ? '0' : '1';
    }

    return received_frame;
}
