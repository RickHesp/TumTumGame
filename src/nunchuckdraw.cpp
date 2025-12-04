#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// If NunchuckJoystick is declared in a C header, ensure C linkage:
#ifdef __cplusplus
extern "C" {
#endif
#include "nunchuck.h"


#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8
#define GRID_SIZE 6
#define SCREEN_SIZE 240
#define CELL_SIZE (SCREEN_SIZE / GRID_SIZE)


Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);



void draw_init() {
    tft.begin();
    tft.setRotation(1); // 320x240 landscape

    const int w = 240;  // square playfield width
    const int h = 240;  // square playfield height
    tft.fillScreen(ILI9341_BLUE);

    const int cell_w = w / 6;
    const int cell_h = h / 6;

    // Vertical lines
    for (int x = 0; x <= w; x += cell_w) {
        int x_clamped = (x >= w) ? (w - 1) : x;
        tft.drawFastVLine(x_clamped, 0, h, ILI9341_BLACK);
    }

    // Horizontal lines
    for (int y = 0; y <= h; y += cell_h) {
        int y_clamped = (y >= h) ? (h - 1) : y;
        tft.drawFastHLine(0, y_clamped, w, ILI9341_BLACK);
    }
}

void draw_cursor(NunchuckJoystick_t joy) {

    // Center of the 240x240 square
    const int centerX = 120;
    const int centerY = 120;

    const float scale = 100.0f / 128.0f;

    int joyX = centerX + static_cast<int>((static_cast<int>(joy.x) - 128) * scale);
    int joyY = centerY + static_cast<int>((static_cast<int>(joy.y) - 128) * scale);

    if (joyX < 0) joyX = 0;
    if (joyX > 239) joyX = 239;
    if (joyY < 0) joyY = 0;
    if (joyY > 239) joyY = 239;

    // Optional: clear previous marker or draw a small contrasting ring
    tft.fillCircle(joyX, joyY, 5, ILI9341_RED);
}

int move_joystick(NunchuckJoystick_t joy) {
    
    int col = (joy.x * GRID_SIZE) / 256;  //collumns
    int row = (joy.y * GRID_SIZE) / 256;  //rows

    // Clamp to grid
    if (col < 0) col = 0;
    if (col >= GRID_SIZE) col = GRID_SIZE - 1;
    if (row < 0) row = 0;
    if (row >= GRID_SIZE) row = GRID_SIZE - 1;

    // Convert (row, col) to index
    return row * GRID_SIZE + col;

}

void drawCubeAtIndex(int index, uint16_t color) {
    int row = index / GRID_SIZE;
    int col = index % GRID_SIZE;

    int x = col * CELL_SIZE;
    int y = row * CELL_SIZE;


    // Draw cube (filled rectangle)
    tft.fillRect(x, y, CELL_SIZE, CELL_SIZE, color);
}

void updateCube(uint8_t expander) {
    NunchuckJoystick_t joy = nunchuck_readJoystick(expander);
    int index = move_joystick(joy);
    drawCubeAtIndex(index, ILI9341_RED);
}

#ifdef __cplusplus
}
#endif