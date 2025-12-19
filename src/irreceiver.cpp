#include "irreceiver.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// Internal buffer and indices
volatile delta_entry_t delta_buffer[BUFFER_SIZE];
volatile uint8_t head = 0;
volatile uint8_t tail = 0;

// Add a delta to the buffer
static inline void buffer_put(uint16_t val, uint8_t state){
    uint8_t next = (head + 1) % BUFFER_SIZE;
    if(next != tail){
        delta_buffer[head].delta = val;
        delta_buffer[head].state = state;
        head = next;
    }
}

// Get a delta from the buffer
int buffer_get(uint16_t *val, uint8_t *state){
    if(head == tail) return 0;
    *val = delta_buffer[tail].delta;
    *state = delta_buffer[tail].state;
    tail = (tail + 1) % BUFFER_SIZE;
    return 1;
}

// Initialize Timer1 for IR pulse timing
void timer1_init(){
    TCCR1A = 0;
    TCCR1B = (1<<CS11); // prescaler 8 → 0.5us per tick
    TCNT1 = 0;
    TIMSK1 = (1<<TOIE1); // enable overflow interrupts on timer1
}

// Initialize INT0 for IR input
void init_receiver(){
    DDRD &= ~(1<<IR_PIN);
    PORTD |= (1<<IR_PIN); // pull-up
    EICRA |= (1<<ISC00);  // INT0 on CHANGE
    EIMSK |= (1<<INT0);
}

// Public initialization function
void init_ir_receiver(){
    timer1_init();
    init_receiver();
}

// ISR for INT0
ISR(INT0_vect){
    static uint16_t last_time = 0;
    uint16_t now = TCNT1;
    uint16_t delta = (now >= last_time) ? (now - last_time) : (0xFFFF - last_time + now);
    last_time = now;

    uint8_t state = (PIND & (1<<IR_PIN)) ? 1 : 0;
    buffer_put(delta, state);
}
