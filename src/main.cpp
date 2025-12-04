#include <brightness.h> 
#include <Adafruit_ILI9341.h>
#include "nunchuck.h"
#include "nunchuckdraw.h"
#include "TWI.h"
#include "draw.h"
#include <Arduino.h>

#define EXPANDER_ADRESS 0x52
uint16_t lastmove = 0;
uint16_t movetime = 100; //time between moves in ms

int main(void){

    init();
    Serial.begin(9600);
    Serial.println("TumTum Game Start");
    grid_init();
    brightness_init();
    nunchuck_init(EXPANDER_ADRESS);
    NunchuckJoystick_t joy;
    while (1)
    {
      unsigned long current = millis();
      if (current - lastmove >= movetime) {
         joy = nunchuck_readJoystick(EXPANDER_ADRESS);   
        int index = move_joysticks(joy, 6);
        color_cell(index, ILI9341_RED);
        lastmove = current;
      }
      read_buttons(EXPANDER_ADRESS, joy.cButton, joy.zButton);
      if (joy.cButton){
        color_cell(1, ILI9341_GREEN);
      }else if(joy.zButton){
          color_cell(1, ILI9341_BLACK);
      }
    }
}