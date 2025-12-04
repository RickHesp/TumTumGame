#include <brightness.h> 
#include <Adafruit_ILI9341.h>
#include "nunchuck.h"
#include "nunchuckdraw.h"
#include "TWI.h"
#include "draw.h"
#include <Arduino.h>

#define EXPANDER_ADRESS 0x52
uint16_t lastmove = 0;
uint16_t movetime = 200; //time between moves in ms

int main(void){

    init();
    Serial.begin(9600);
    Serial.println("TumTum Game Start");
    grid_init();
    brightness_init();
    nunchuck_init(EXPANDER_ADRESS);
    while (1)
    {
      unsigned long current = millis();
      if (current - lastmove >= movetime) {
        NunchuckJoystick_t joy = nunchuck_readJoystick(EXPANDER_ADRESS);
        int index = move_joysticks(joy, 6);
        color_cell(index, ILI9341_RED);
        lastmove = current;
    }
      // serial debugging
      // Serial.print("Cell Index: ");
      // Serial.print(index);
      // Serial.print("Joystick X: ");
      // Serial.print(joy.x);
      // Serial.print(" | Joystick Y: ");
      // Serial.println(joy.y);

      // delay(10);
    }
}
