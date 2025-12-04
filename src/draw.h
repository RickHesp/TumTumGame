#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "nunchuck.h"
#include "nunchuckdraw.h"

#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);



void grid_init(){
    tft.begin();
    tft.setRotation(1); //320x240

    //make the play field a square (240x240)
    int w = 240; //240px
    int h = tft.height(); //240px

    int horizontal_val = w/6;
    int vertical_val = h/6;

    //start drawing (6x6 field)
    tft.fillScreen(ILI9341_BLUE);
    for (int i = 0; i <= w; i+=vertical_val) {//make vertical lines all h/6 apart
        tft.drawFastVLine(i, 0, h, ILI9341_BLACK);
    }
    
    for(int i = 0; i <= h; i+=horizontal_val){ //make horizontal lines all w/6 apart
        tft.drawFastHLine(0, i, w, ILI9341_BLACK);
    }
}

void color_cell(uint8_t cell, uint16_t color)
{
    //check valid cell
    if(cell < 1 || cell > 36) return;

    //calculate row and column
    uint8_t index = cell;
    uint8_t row   = index / 6;
    uint8_t col   = index % 6;

    //calculate cell size
    uint16_t cellSize = 240 / 6;

    //calculate x and y position
    uint16_t x = col * cellSize;
    uint16_t y = row * cellSize;

    //fill 1 cell (slightly smaller so lines remain visible)
    tft.fillRect(x+1, y+1, cellSize-1, cellSize-1, color);
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