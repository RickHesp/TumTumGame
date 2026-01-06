#include <avr/io.h>
#include <avr/interrupt.h>

#define HALFBIT_US 889
#define IR_PIN PD2
#define BUFFER_SIZE 64

typedef struct {
    uint16_t delta;
    uint8_t state; // 0 = LOW, 1 = HIGH
} delta_entry_t;

extern volatile delta_entry_t delta_buffer[BUFFER_SIZE];
extern volatile uint8_t head;
extern volatile uint8_t tail;

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

void timer1_init();
void init_receiver();
void init_ir_receiver();