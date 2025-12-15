#include <avr/io.h>
#include <avr/interrupt.h>
#include "sendcommand.h"
#include "micros_timer.h"

#define IRLED DDD6
#define ACK_TIMEOUT 500000//500 ms

volatile uint16_t frame;
volatile uint16_t ack_frame;
volatile uint16_t command_frame;

volatile uint8_t half_bit_index = 0;
volatile uint8_t sending = 0;
volatile uint8_t toggle_bit = 0;

bool wait_ack = false;
uint32_t last_send_time = 0;


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
    if(wait_ack && sending){
        return;//previous send not complete
    }
    wait_ack = true;
    frame = build_frame(field, address, command, toggle_bit);
    half_bit_index = 0;
    sending = 1;
    last_send_time = micros_timer();
}

void send_ack(){
    ack_frame = build_frame(1, 31, 63, toggle_bit);
    command_frame= frame;
    send_command(1, 31, 63); //ACK command
}

void await_ack(){
    if (!wait_ack) return;
    if (sending) return;//wait until sending done
    uint32_t now = micros_timer();
    if (now - last_send_time >= ACK_TIMEOUT) {
        frame = command_frame;
        half_bit_index = 0;
        sending = 1;
        last_send_time = now;
    }
}


void receive_acknowledgement(){
    wait_ack=false;
    
}

ISR(TIMER2_COMPA_vect)
{
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
}

void init_ir_sender(){
    init_carrier();
    init_sender();
}