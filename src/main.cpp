#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <usart.h>

#define HALFBIT_US 889
#define IRLED DDD6
#define IR_PIN PD2
#define BUFFER_SIZE 64

// Sender
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
