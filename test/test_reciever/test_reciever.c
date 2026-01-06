#include <unity.h>
#include <stdint.h>
#include "irreceiver.h"


// === setup tests ===//
void setup(void) {
    head = 0;
    tail = 0;
    for(int i=0; i<BUFFER_SIZE; i++){
        delta_buffer[i].delta = 0;
        delta_buffer[i].state = 0;
    }
}

// --- Buffer tests ---
void test_buffer_put_and_get_should_store_and_retrieve(void) {
    buffer_put(1234, 1);
    uint16_t val;
    uint8_t state;
    TEST_ASSERT_EQUAL(1, buffer_get(&val, &state));
    TEST_ASSERT_EQUAL_UINT16(1234, val);
    TEST_ASSERT_EQUAL_UINT8(1, state);
}

void test_buffer_get_empty_should_return_zero(void) {
    uint16_t val;
    uint8_t state;
    TEST_ASSERT_EQUAL(0, buffer_get(&val, &state));
}

// --- Timer init test ---
void test_timer1_init_should_set_registers(void) {
    timer1_init();
    TEST_ASSERT_EQUAL_UINT8(0, TCCR1A);
    TEST_ASSERT_EQUAL_UINT8((1<<CS11), TCCR1B);
    TEST_ASSERT_EQUAL_UINT16(30, TCNT1);
    TEST_ASSERT_EQUAL_UINT8((1<<TOIE1), TIMSK1);
}

// --- Receiver init test ---
void test_init_receiver_should_configure_pins_and_interrupts(void) {
    DDRD = 0xFF;   // alles output
    PORTD = 0x00;
    EICRA = 0x00;
    EIMSK = 0x00;

    init_receiver();

    TEST_ASSERT_EQUAL_UINT8(0xFF & ~(1<<IR_PIN), DDRD); // IR_PIN input
    TEST_ASSERT_TRUE(PORTD & (1<<IR_PIN));              // pull-up enabled
    TEST_ASSERT_TRUE(EICRA & (1<<ISC00));               // interrupt on change
    TEST_ASSERT_TRUE(EIMSK & (1<<INT0));                // INT0 enabled
}

// --- ISR test ---
void test_ISR_should_store_delta_and_state(void) {
    TCNT1 = 1000;
    PIND = (1<<IR_PIN); // simulate HIGH
    INT0_vect();        // call ISR manually

    uint16_t val;
    uint8_t state;
    TEST_ASSERT_EQUAL(1, buffer_get(&val, &state));
    TEST_ASSERT_EQUAL_UINT16(1005, val); // first delta = now - 0
    TEST_ASSERT_EQUAL_UINT8(1, state);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_buffer_put_and_get_should_store_and_retrieve);
    RUN_TEST(test_buffer_get_empty_should_return_zero);
    RUN_TEST(test_timer1_init_should_set_registers);
    RUN_TEST(test_init_receiver_should_configure_pins_and_interrupts);
    RUN_TEST(test_ISR_should_store_delta_and_state);
    return UNITY_END();
}