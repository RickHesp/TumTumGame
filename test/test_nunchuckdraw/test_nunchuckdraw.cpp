#include "unity.h"
#include "nunchuckdraw.h"
#include "micros_timer.h"
#include "display.h"

// External variables from nunchuckdraw.c
extern int start_index;
extern NunchuckJoystick_t joy;
extern uint16_t index;
extern uint16_t old_index;
extern uint32_t last_time;

// Define constants that should match your actual code
#define CENTER_X 128
#define CENTER_Y 128
#define MARGIN 5
#define MOVE_DELAY 1

// Assuming GRID dimensions - adjust to match your actual values
#ifndef GRID_WIDTH
#define GRID_WIDTH 10
#endif
#ifndef GRID_HEIGHT
#define GRID_HEIGHT 10
#endif

void setUp(void) {
    // Reset state before each test
    start_index = 0;
    index = 0;
    old_index = 0;
    last_time = 0;
}

void tearDown(void) {
    // Clean up after each test
}

// ===== move_joysticks() Tests =====

void test_move_joysticks_no_movement_when_centered(void) {
    start_index = 55;
    NunchuckJoystick_t joy = {CENTER_X, CENTER_Y, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(55, result);
    TEST_ASSERT_EQUAL_INT(55, start_index);
}

void test_move_joysticks_no_movement_within_margin(void) {
    start_index = 55;
    NunchuckJoystick_t joy = {CENTER_X + MARGIN - 1, CENTER_Y + MARGIN - 1, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(55, result);
    TEST_ASSERT_EQUAL_INT(55, start_index);
}

void test_move_joysticks_move_right(void) {
    start_index = 55; // Row 5, Col 5
    NunchuckJoystick_t joy = {CENTER_X + MARGIN + 10, CENTER_Y, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(56, result);
    TEST_ASSERT_EQUAL_INT(56, start_index);
}

void test_move_joysticks_move_left(void) {
    start_index = 55;
    NunchuckJoystick_t joy = {CENTER_X - MARGIN - 10, CENTER_Y, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(54, result);
    TEST_ASSERT_EQUAL_INT(54, start_index);
}

void test_move_joysticks_move_down(void) {
    start_index = 55;
    NunchuckJoystick_t joy = {CENTER_X, CENTER_Y - MARGIN - 10, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(65, result); // Row 6, Col 5
    TEST_ASSERT_EQUAL_INT(65, start_index);
}

void test_move_joysticks_move_up(void) {
    start_index = 55;
    NunchuckJoystick_t joy = {CENTER_X, CENTER_Y + MARGIN + 10, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(45, result); // Row 4, Col 5
    TEST_ASSERT_EQUAL_INT(45, start_index);
}

void test_move_joysticks_wrap_right_edge(void) {
    start_index = 59; // Row 5, Col 9
    NunchuckJoystick_t joy = {CENTER_X + MARGIN + 10, CENTER_Y, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(50, result); // Wrap to Row 5, Col 0
    TEST_ASSERT_EQUAL_INT(50, start_index);
}

void test_move_joysticks_wrap_left_edge(void) {
    start_index = 50; // Row 5, Col 0
    NunchuckJoystick_t joy = {CENTER_X - MARGIN - 10, CENTER_Y, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(59, result); // Wrap to Row 5, Col 9
    TEST_ASSERT_EQUAL_INT(59, start_index);
}

void test_move_joysticks_wrap_bottom_edge(void) {
    start_index = 95; // Row 9, Col 5
    NunchuckJoystick_t joy = {CENTER_X, CENTER_Y - MARGIN - 10, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(5, result); // Wrap to Row 0, Col 5
    TEST_ASSERT_EQUAL_INT(5, start_index);
}

void test_move_joysticks_wrap_top_edge(void) {
    start_index = 5; // Row 0, Col 5
    NunchuckJoystick_t joy = {CENTER_X, CENTER_Y + MARGIN + 10, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(95, result); // Wrap to Row 9, Col 5
    TEST_ASSERT_EQUAL_INT(95, start_index);
}

void test_move_joysticks_priority_to_larger_x_movement(void) {
    start_index = 55;
    NunchuckJoystick_t joy = {CENTER_X + 20, CENTER_Y + 10, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(56, result); // X movement wins
    TEST_ASSERT_EQUAL_INT(56, start_index);
}

void test_move_joysticks_priority_to_larger_y_movement(void) {
    start_index = 55;
    NunchuckJoystick_t joy = {CENTER_X + 10, CENTER_Y + 20, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(45, result); // Y movement wins
    TEST_ASSERT_EQUAL_INT(45, start_index);
}

void test_move_joysticks_equal_xy_movement_x_priority(void) {
    start_index = 55;
    NunchuckJoystick_t joy = {CENTER_X + 15, CENTER_Y + 15, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(56, result); // X has priority with >=
    TEST_ASSERT_EQUAL_INT(56, start_index);
}

void test_move_joysticks_corner_top_left(void) {
    start_index = 0;
    NunchuckJoystick_t joy = {CENTER_X - MARGIN - 10, CENTER_Y, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(9, result); // Wrap to Col 9
    TEST_ASSERT_EQUAL_INT(9, start_index);
}

void test_move_joysticks_corner_bottom_right(void) {
    start_index = 99;
    NunchuckJoystick_t joy = {CENTER_X + MARGIN + 10, CENTER_Y, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(90, result); // Wrap to Col 0
    TEST_ASSERT_EQUAL_INT(90, start_index);
}

void test_move_joysticks_maximum_joystick_value(void) {
    start_index = 55;
    NunchuckJoystick_t joy = {255, CENTER_Y, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(56, result);
    TEST_ASSERT_EQUAL_INT(56, start_index);
}

void test_move_joysticks_minimum_joystick_value(void) {
    start_index = 55;
    NunchuckJoystick_t joy = {0, CENTER_Y, false, false};
    
    uint16_t result = move_joysticks(joy);
    
    TEST_ASSERT_EQUAL_UINT16(54, result);
    TEST_ASSERT_EQUAL_INT(54, start_index);
}

// ===== joystick_select() Tests =====

void test_joystick_select_updates_index_after_delay(void) {
    start_index = 55;
    last_time = 0;
    NunchuckJoystick_t test_joy = {CENTER_X + 20, CENTER_Y, false, false};
    
    micros_timer_ExpectAndReturn(100);
    nunchuck_readJoystick_ExpectAndReturn(test_joy);
    selectCell_Expect(56);
    deselectCell_Expect(55);
    
    uint16_t result = joystick_select();
    
    TEST_ASSERT_EQUAL_UINT16(56, result);
    TEST_ASSERT_EQUAL_UINT16(56, index);
    TEST_ASSERT_EQUAL_UINT32(100, last_time);
}

void test_joystick_select_no_update_within_delay(void) {
    start_index = 55;
    index = 55;
    last_time = 100;
    
    micros_timer_ExpectAndReturn(100); // Same time, within delay
    
    uint16_t result = joystick_select();
    
    TEST_ASSERT_EQUAL_UINT16(55, result);
    TEST_ASSERT_EQUAL_UINT16(55, index);
}

void test_joystick_select_multiple_movements(void) {
    start_index = 55;
    last_time = 0;
    NunchuckJoystick_t test_joy1 = {CENTER_X + 20, CENTER_Y, false, false};
    NunchuckJoystick_t test_joy2 = {CENTER_X - 20, CENTER_Y, false, false};
    
    // First movement
    micros_timer_ExpectAndReturn(100);
    nunchuck_readJoystick_ExpectAndReturn(test_joy1);
    selectCell_Expect(56);
    deselectCell_Expect(55);
    
    joystick_select();
    TEST_ASSERT_EQUAL_UINT16(56, index);
    
    // Second movement
    micros_timer_ExpectAndReturn(200);
    nunchuck_readJoystick_ExpectAndReturn(test_joy2);
    selectCell_Expect(55);
    deselectCell_Expect(56);
    
    uint16_t result = joystick_select();
    TEST_ASSERT_EQUAL_UINT16(55, result);
}

// ===== nunchuck_place_boat() Tests =====

void test_nunchuck_place_boat_returns_true_when_z_pressed(void) {
    joy.zButton = true;
    index = 42;
    
    placeBoat_Expect(42);
    
    bool result = nunchuck_place_boat();
    
    TEST_ASSERT_TRUE(result);
}

void test_nunchuck_place_boat_returns_false_when_z_not_pressed(void) {
    joy.zButton = false;
    
    bool result = nunchuck_place_boat();
    
    TEST_ASSERT_FALSE(result);
}

void test_nunchuck_place_boat_multiple_positions(void) {
    // Test at different positions
    joy.zButton = true;
    index = 0;
    placeBoat_Expect(0);
    TEST_ASSERT_TRUE(nunchuck_place_boat());
    
    index = 99;
    placeBoat_Expect(99);
    TEST_ASSERT_TRUE(nunchuck_place_boat());
}

// ===== Test Runner =====

int main(void) {
    UNITY_BEGIN();
    
    // move_joysticks() tests
    RUN_TEST(test_move_joysticks_no_movement_when_centered);
    RUN_TEST(test_move_joysticks_no_movement_within_margin);
    RUN_TEST(test_move_joysticks_move_right);
    RUN_TEST(test_move_joysticks_move_left);
    RUN_TEST(test_move_joysticks_move_down);
    RUN_TEST(test_move_joysticks_move_up);
    RUN_TEST(test_move_joysticks_wrap_right_edge);
    RUN_TEST(test_move_joysticks_wrap_left_edge);
    RUN_TEST(test_move_joysticks_wrap_bottom_edge);
    RUN_TEST(test_move_joysticks_wrap_top_edge);
    RUN_TEST(test_move_joysticks_priority_to_larger_x_movement);
    RUN_TEST(test_move_joysticks_priority_to_larger_y_movement);
    RUN_TEST(test_move_joysticks_equal_xy_movement_x_priority);
    RUN_TEST(test_move_joysticks_corner_top_left);
    RUN_TEST(test_move_joysticks_corner_bottom_right);
    RUN_TEST(test_move_joysticks_maximum_joystick_value);
    RUN_TEST(test_move_joysticks_minimum_joystick_value);
    
    // joystick_select() tests
    RUN_TEST(test_joystick_select_updates_index_after_delay);
    RUN_TEST(test_joystick_select_no_update_within_delay);
    RUN_TEST(test_joystick_select_multiple_movements);
    
    // nunchuck_place_boat() tests
    RUN_TEST(test_nunchuck_place_boat_returns_true_when_z_pressed);
    RUN_TEST(test_nunchuck_place_boat_returns_false_when_z_not_pressed);
    RUN_TEST(test_nunchuck_place_boat_multiple_positions);
    
    return UNITY_END();
}