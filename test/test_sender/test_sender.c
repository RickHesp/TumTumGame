#include <unity.h>
#include "sendcommand.h"

// === setup tests ===//
void setup(void) {
    DDRD = TCCR0A = TCCR0B = OCR0A = 0;
    TCCR2A = TCCR2B = OCR2A = TIMSK2 = 0;
    frame = 0;
    half_bit_index = 0;
    sending = 0;
    toggle_bit = 0;
}

// === test init carrier function ===//
void test_init_carrier_should_configure_timer0(void) {
    init_carrier();
    TEST_ASSERT_TRUE(DDRD & (1 << IRLED));
    TEST_ASSERT_EQUAL_UINT8((1 << COM0A0) | (1 << WGM01), TCCR0A);
    TEST_ASSERT_EQUAL_UINT8((1 << WGM02) | (1 << CS00), TCCR0B);
    TEST_ASSERT_EQUAL_UINT8(210, OCR0A);
}

// === test init sender function ===//
void test_init_sender_should_configure_timer2(void) {
    init_sender();
    TEST_ASSERT_EQUAL_UINT8((1 << WGM21), TCCR2A);
    TEST_ASSERT_EQUAL_UINT8((1 << CS22), TCCR2B);
    TEST_ASSERT_EQUAL_UINT8(222, OCR2A);
    TEST_ASSERT_EQUAL_UINT8((1 << OCIE2A), TIMSK2);
}

// === test frames ===//
void test_build_frame_should_construct_correct_frame(void) {
    uint16_t f = build_frame(1, 0x1A, 0x2B, 1);
    TEST_ASSERT_TRUE(f & (1 << 13)); // Startbit
    TEST_ASSERT_TRUE(f & (1 << 12)); // Fieldbit
    TEST_ASSERT_TRUE(f & (1 << 11)); // Togglebit
    TEST_ASSERT_EQUAL_UINT16(0x1A << 6, f & (0x1F << 6)); // Address
    TEST_ASSERT_EQUAL_UINT16(0x2B, f & 0x3F); // Command
}

// === test send command ===//
void test_send_command_should_set_frame_and_flags(void) {
    toggle_bit = 1;
    send_command(0, 0x05, 0x12);
    TEST_ASSERT_EQUAL_UINT16(build_frame(0, 0x05, 0x12, 1), frame);
    TEST_ASSERT_EQUAL_UINT8(0, half_bit_index);
    TEST_ASSERT_EQUAL_UINT8(1, sending);
}

//=== ISR test ===//
void test_TIMER2_COMPA_vect_should_transmit_bits_and_toggle(void) {
    frame = build_frame(0, 0x01, 0x01, 0);
    sending = 1;
    half_bit_index = 0;

    for (int i = 0; i < 28; i++) {
        TCCR0A = 0xFF; // simulate carrier
        TIMER2_COMPA_vect();
    }

    TEST_ASSERT_EQUAL_UINT8(0, sending);
    TEST_ASSERT_EQUAL_UINT8(0, half_bit_index);
    TEST_ASSERT_FALSE(TCCR0A & (1 << COM0A1)); // carrier off
}

//=== run tests ===//
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_init_carrier_should_configure_timer0);
    RUN_TEST(test_init_sender_should_configure_timer2);
    RUN_TEST(test_build_frame_should_construct_correct_frame);
    RUN_TEST(test_send_command_should_set_frame_and_flags);
    RUN_TEST(test_TIMER2_COMPA_vect_should_transmit_bits_and_toggle);
    return UNITY_END();
}