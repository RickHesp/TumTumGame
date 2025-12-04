#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"   // contains USART_PRINT()

#define IR_PIN PD2
#define BUFFER_SIZE 64

typedef struct {
    uint16_t delta;
    uint8_t state;
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

void timer1_init(void){
    TCCR1A = 0;
    TCCR1B = (1<<CS11); // prescaler 8 → 0.5us per tick
    TCNT1 = 0;
}

void init_ir_receiver(void){
    DDRD &= ~(1<<IR_PIN);
    PORTD |= (1<<IR_PIN); // pull-up
    EICRA |= (1<<ISC00);  // INT0 on CHANGE
    EIMSK |= (1<<INT0);   // enable INT0
    timer1_init();
}

ISR(INT0_vect){
    static uint16_t last_time = 0;
    uint16_t now = TCNT1;
    uint16_t delta = (now >= last_time) ? (now - last_time) : (0xFFFF - last_time + now);
    last_time = now;

    uint8_t state = (PIND & (1<<IR_PIN)) ? 1 : 0;
    buffer_put(delta, state);
}
void uart_putc(char c){
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
}
/*
 * scanSignal: processes the IR buffer, decodes half-bits into full bits,
 * and prints each frame via USART_PRINT().
 */
void scanSignal(void){
            static char halfbits[32];
    static uint8_t halfcount = 0;

        uint16_t delta;
        uint8_t state;
        while(buffer_get(&delta, &state)){
          //new frame
            if(delta > 5000){
                for(uint8_t i=0; i+1<halfcount; i+=2){
                    if(halfbits[i] == '0' && halfbits[i+1] == '1') USART_Print("0");
                    else if(halfbits[i] == '1' && halfbits[i+1] == '0') USART_Print("1");
                    else USART_Print("?"); // invalid pair
                }
                if(halfcount % 2 == 1){
                    uart_putc(halfbits[halfcount-1]);
                }

                USART_Print("\n");
                halfcount = 0; // reset for next frame
                continue;
            }

            // Determine number of half-bits
            uint8_t count = 0;
            if(delta < 2200) count = 1;      // short pulse
            else if(delta > 2800) count = 2; // long pulse

            for(uint8_t i=0; i<count && halfcount<32; i++){
                halfbits[halfcount++] = state ? '0' : '1';
            }
        }
}
