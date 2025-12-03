#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <usart.h>

#define HALFBIT_US 889
#define BUFFER_SIZE 64

volatile uint8_t ir_buffer[BUFFER_SIZE];
volatile uint8_t ir_head = 0;
volatile uint8_t ir_tail = 0;

// sender
// Timer0
volatile uint16_t frame;
volatile uint8_t send_next_command_flag = 0;
volatile uint8_t half_bit_index = 0;
volatile uint8_t sending = 0;
volatile uint8_t toggle_bit = 0;

void init_carrier() {
    DDRD |= (1<<DDD6); 

    TCCR0A = (1<<COM0A1)|(1<<WGM01)|(1<<WGM00); 
    OCR0A = 210; // ~38 kHz
    TCCR0B = (1<<CS00); // no prescaler
}

// Timer2
volatile uint8_t tx_byte = 0x55; // 01010101
volatile uint8_t tx_bit_index = 0;
volatile uint8_t tx_busy = 0;

void init_sender() {
    TCCR2A = 0;
    TCCR2B = 0;
    TCCR2A = (1<<WGM21); // CTC
    OCR2A = 222; // half bit: 889us
    TCCR2B = (1<<CS22);   // prescaler 64
    TIMSK2 = (1<<OCIE2A); // enable compare match interrupt
}

void send_command(){
    USART_Print("Sending...");
    frame = 0b10101000111010; 
    half_bit_index = 0;
    sending = 1;
}

ISR(TIMER2_COMPA_vect) {
    static uint16_t software_counter = 0;

    if (!sending) {
        TCCR0A &= ~(1<<COM0A1); // carrier off
    } else {
        uint8_t bit_index = half_bit_index / 2;
        uint8_t first_half = (half_bit_index % 2 == 0);
        uint8_t bit_value = (frame >> (13 - bit_index)) & 1;

        if ((bit_value && first_half) || (!bit_value && !first_half))
            TCCR0A |= (1<<COM0A1);  // carrier on
        else
            TCCR0A &= ~(1<<COM0A1); // carrier off

        half_bit_index++;
        if (half_bit_index >= 28) {
            half_bit_index = 0;
            sending = 0;
            toggle_bit ^= 1;
            TCCR0A &= ~(1<<COM0A1);
        }
    }

    software_counter++;
    if(software_counter > 1125){ // 1125 * 889us = 1s
        software_counter = 0;
        send_next_command_flag = 1;
    }
}

// Timer1
void init_ir_receiver() {
    DDRD &= ~(1<<DDD2);   // PD2 input
    PORTD |= (1<<PORTD2); // pull-up

    TCCR1B = (1<<CS10);   // timer1 start, prescaler 1
    EICRA = (1<<ISC00);   // INT0 trigger ANY CHANGE
    EIMSK = (1<<INT0);    // enable INT0
}

ISR(INT0_vect) {
    static uint16_t last_count = 0;
    uint16_t now = TCNT1;
    uint16_t pulse = now - last_count;
    last_count = now;

    if(pulse > (HALFBIT_US*16)) { // timer1 1us
        ir_buffer[ir_head] = 1;
    } else {
        ir_buffer[ir_head] = 0;
    }

    ir_head = (ir_head + 1) % BUFFER_SIZE;
}
uint8_t n = 0;
uint16_t counter = 0;
int main(void) {
    USART_Init();
    init_carrier();
    init_sender();
    init_ir_receiver();
    sei(); 

    USART_Print("IR monitor started\r\n");

    while(1) {
        if(send_next_command_flag){
            send_next_command_flag = 0;
            send_command();
        }

        // buffer uitlezen en printen
        while(ir_tail != ir_head) {
            if(ir_buffer[ir_tail])
                USART_Transmit('1');
            else
                USART_Transmit('0');
            ir_tail = (ir_tail + 1) % BUFFER_SIZE;
                    n++;
        if(n >= 4) {
            counter++;
            n = 0;
            char buf[10];
            itoa(counter, buf, 10);
            USART_Print(buf);
            USART_Print("\r\n");
          }
        }
    }
}
