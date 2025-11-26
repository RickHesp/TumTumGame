#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <HardwareSerial.h>

#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8

// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST); //setup adafruit constructor

// void setup() {
//   tft.begin();
//   tft.setRotation(1); //landscape

//   int w = tft.width(); 
//   int h = tft.height();
//   for (int i = 0; i <= 10; i++) {
//     int x = map(i, 0, 10, 0, w - 1);
//     int y = map(i, 0, 10, 0, h - 1);
//     if(!(i%2)){
//         Serial.println("if");
//         tft.drawLine(y, 0, y, w - 1, ILI9341_BLACK);
//     }else{
//         Serial.println("else");
//         tft.drawLine(x, 0, x, h - 1, ILI9341_BLACK);
//     }
//   }
// }

// void loop() {
    
// }