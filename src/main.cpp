#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/16/BAUD)-1)
#define BITTIME_US 1000   // 1 ms per bit
#define BUFFER_SIZE 64

volatile uint8_t ir_buffer[BUFFER_SIZE];
volatile uint8_t ir_head = 0;
volatile uint8_t ir_tail = 0;

// UART
void uart_init(void) {
    UBRR0H = (UBRR_VALUE >> 8);
    UBRR0L = UBRR_VALUE;
    UCSR0B = (1<<TXEN0);
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void uart_send(char c) {
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
}

void uart_print(const char* s) {
    while(*s) uart_send(*s++);
}

// Timer0
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
    // Timer2 CTC mode
    TCCR2A = (1<<WGM21); // CTC
    OCR2A = 199;
    TCCR2B = (1<<CS21);   // prescaler 8
    TIMSK2 = (1<<OCIE2A); // enable compare match interrupt
}

ISR(TIMER2_COMPA_vect) {
    if(tx_bit_index < 8) {
        if(tx_byte & (1<<tx_bit_index)) {
            TCCR0A |= (1<<COM0A1);
        } else {
            TCCR0A &= ~(1<<COM0A1);
        }
        tx_bit_index++;
    } else {
        TCCR0A &= ~(1<<COM0A1); 
        tx_bit_index = 0;
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

    if(pulse > (BITTIME_US*16)) { // timer1 1us
        ir_buffer[ir_head] = 1;
    } else {
        ir_buffer[ir_head] = 0;
    }

    ir_head = (ir_head + 1) % BUFFER_SIZE;
}
uint8_t n = 0;
int counter = 0;
int main(void) {
    uart_init();
    init_carrier();
    init_sender();
    init_ir_receiver();
    sei(); 

    uart_print("IR monitor started\r\n");

    while(1) {
        // buffer uitlezen en printen
        while(ir_tail != ir_head) {
            if(ir_buffer[ir_tail])
                uart_send('1');
            else
                uart_send('0');
            ir_tail = (ir_tail + 1) % BUFFER_SIZE;
                    n++;
        if(n >= 4) {
            counter++;
            n = 0;
            char buf[10];
            itoa(counter, buf, 10);
            uart_print(buf);
            uart_print("\r\n");
          }
        }
    }
}
