#include "irreceiver.h"

volatile delta_entry_t delta_buffer[BUFFER_SIZE];
volatile uint8_t head = 0;
volatile uint8_t tail = 0;

void timer1_init(){
    TCCR1A = 0;
    TCCR1B = (1<<CS11); // prescaler 8 → 0.5us per tick
    TCNT1 = 0;
    TIMSK1 = (1<<TOIE1);//enable overflow interrupts on timer1
}

void init_receiver(){
    DDRD &= ~(1<<IR_PIN);
    PORTD |= (1<<IR_PIN); // pull-up
    EICRA |= (1<<ISC00);  // INT0 on CHANGE
    EIMSK |= (1<<INT0);
}

ISR(INT0_vect){
    static uint16_t last_time = 0;
    uint16_t now = TCNT1;
    uint16_t delta = (now >= last_time) ? (now - last_time) : (0xFFFF - last_time + now);
    last_time = now;

    uint8_t state = (PIND & (1<<IR_PIN)) ? 1 : 0;
    buffer_put(delta, state);
}

void init_ir_receiver(){
    timer1_init();
    init_receiver();
}