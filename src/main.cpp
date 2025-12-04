#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <usart.h>
#include "sendcommand.h"

#define HALFBIT_US 889
#define IR_PIN PD2
#define BUFFER_SIZE 64

// Receiver
typedef struct {
    uint16_t delta;
    uint8_t state; // 0 = LOW, 1 = HIGH
} delta_entry_t;

volatile delta_entry_t delta_buffer[BUFFER_SIZE];
volatile uint8_t head = 0;
volatile uint8_t tail = 0;

static inline void buffer_put(uint16_t val, uint8_t state){
    uint8_t next = (head + 1) % BUFFER_SIZE;
    if(next != tail){
        delta_buffer[head].delta = val;
        delta_buffer[head].state = state;
        head = next;
    }
}

static inline int buffer_get(uint16_t *val, uint8_t *state){
    if(head == tail) return 0;
    *val = delta_buffer[tail].delta;
    *state = delta_buffer[tail].state;
    tail = (tail + 1) % BUFFER_SIZE;
    return 1;
}

void timer1_init(){
    TCCR1A = 0;
    TCCR1B = (1<<CS11); // prescaler 8 → 0.5us per tick
    TCNT1 = 0;
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

int main(void){
    USART_Init();
    USART_Print("IR sender/receiver ready\r\n");

    init_carrier();
    init_sender();
    timer1_init();
    init_receiver();
    sei();

    char halfbits[32];
    uint8_t halfcount = 0;

    while(1){
        if(send_next_command_flag){
            send_next_command_flag = 0;
            send_command(0, 1, 0x65);
        }

        uint16_t delta;
        uint8_t state;
        while(buffer_get(&delta, &state)){
            if(delta > 6000){
                for(uint8_t i=0; i+1<halfcount; i+=2){
                    if(halfbits[i]=='0' && halfbits[i+1]=='1') USART_putc('0');
                    else if(halfbits[i]=='1' && halfbits[i+1]=='0') USART_putc('1');
                    else USART_putc('?');
                }
                if(halfcount%2==1) USART_putc(halfbits[halfcount-1]);
                USART_putc('\n');
                halfcount = 0;
                continue;
            }

            uint8_t count = 0;
            if(delta < 2400) count = 1;
            else if(delta > 2800) count = 2;

            for(uint8_t i=0; i<count && halfcount<32; i++)
                halfbits[halfcount++] = state ? '0' : '1';
        }
    }
}
