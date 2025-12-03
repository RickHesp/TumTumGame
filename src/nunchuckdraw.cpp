#include "Adafruit_ILI9341.h"
#include <Adafruit_GFX.h>
#include "nunchuck.h"

#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

void draw_init() {
    tft.begin();
    tft.setRotation(1); // 320x240

    int w = 240;
    int h = 240;
    tft.fillScreen(ILI9341_BLUE);

    int horizontal_val = w / 6;
    int vertical_val = h / 6;

    for (int i = 0; i < w; i += vertical_val) {
        tft.drawFastVLine(i, 0, h, ILI9341_BLACK);
    }

    for (int i = 0; i < h; i += horizontal_val) {
        tft.drawFastHLine(0, i, w, ILI9341_BLACK);
    }
}

// void draw_joystick(NunchuckJoystick joy) {
//
//     int centerX = 120;
//     int centerY = 120;
//     int scale = 100 / 32; // Scale joystick values to fit the screen
//
//     int joyX = centerX + (joy.x - 128) * scale;
//     int joyY = centerY + (joy.y - 128) * scale;
//
//     tft.fillCircle(joyX, joyY, 5, ILI9341_RED);
// }

void draw_joystick(NunchuckJoystick joy) {
    // Example: draw a dot at joystick position
    tft.fillCircle(joy.x, joy.y, 3, ILI9341_RED);
}