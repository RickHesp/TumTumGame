#include <avr/io.h>
#include <avr/interrupt.h>

#define IRLED DDD6

volatile uint16_t frame;       
volatile uint8_t half_bit_index = 0;
volatile uint8_t sending = 0;
volatile uint8_t toggle_bit = 0;
volatile uint8_t send_next_command_flag = 0;

void init_carrier() {
    DDRD |= (1 << IRLED);       // PD6 = OC0A
    TCCR0A = (1<<COM0A0) | (1<<WGM01); // toggle on compare match, CTC
    TCCR0B = (1<<WGM02) | (1<<CS00);   // no prescaler
    OCR0A = 210;  // ~38 kHz
}

void init_sender() {
    TCCR2A = 0;
    TCCR2B = 0;
    TCCR2A = (1<<WGM21); // CTC
    OCR2A = 222; // half bit 889us
    TCCR2B = (1<<CS22); // prescaler 64
    TIMSK2 = (1<<OCIE2A); // enable compare match interrupt
}

uint16_t build_frame(uint8_t field, uint8_t address, uint8_t command, uint8_t toggle) {
    uint16_t f = 0;
    f |= (1 << 13); // Startbit
    f |= (field & 1) << 12; // Fieldbit
    f |= (toggle & 1) << 11; // Togglebit
    f |= (address & 0x1F) << 6; // A0–A4
    f |= (command & 0x3F); // C1–C6
    return f;
}

void send_command(uint8_t field, uint8_t address, uint8_t command)
{
    frame = build_frame(field, address, command, toggle_bit);
    half_bit_index = 0;
    sending = 1;
}

ISR(TIMER2_COMPA_vect)
{
    static uint16_t software_counter = 0;

    if (!sending) {
        TCCR0A &= ~(1<<COM0A1); // carrier uit
    } else {
        uint8_t bit_index = half_bit_index / 2;
        uint8_t first_half = (half_bit_index % 2 == 0);
        uint8_t bit_value = (frame >> (13 - bit_index)) & 1;

        if ((bit_value && first_half) || (!bit_value && !first_half))
            TCCR0A |= (1<<COM0A1);  // carrier aan
        else
            TCCR0A &= ~(1<<COM0A1); // carrier uit

        half_bit_index++;
        if (half_bit_index >= 28) {
            half_bit_index = 0;
            sending = 0;
            toggle_bit ^= 1;
            TCCR0A &= ~(1<<COM0A1);
        }
    }

    // software timer voor periodiek zenden (1s)
    software_counter++;
    if(software_counter >= 1125){ // 1125 * 889us ≈ 1 s
        software_counter = 0;
        send_next_command_flag = 1;
    }
}

void init_ir_sender(){
    init_carrier();
    init_sender();
}