#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);



void MakeGrid(){
    tft.begin();
    tft.setRotation(1);

    int w = tft.width();
    int h = tft.height();

    int horizontal_val = h/7;
    int vertical_val = h/7;

    //start drawing
    tft.fillScreen(ILI9341_BLUE);
    for (int i = 0; i <= w; i+=vertical_val) {
        tft.drawFastVLine(i, 0, h, ILI9341_BLACK);
    }
    
    for(int i = 0; i <= h; i+=horizontal_val){
        tft.drawFastHLine(0, i, w, ILI9341_BLACK);
    }
}