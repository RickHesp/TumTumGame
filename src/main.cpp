#include <brightness.h> 
#include <Adafruit_ILI9341.h>
#include "nunchuck.h"
#include "nunchuckdraw.h"
#include "TWI.h"
#include "draw.h"

#define EXPANDER_ADRESS 0x21

int main(void){
    init();
    grid_init();
    brightness_init();
    nunchuck_init(EXPANDER_ADRESS);
    while (1)
    {
      int index = move_joysticks(nunchuck_readJoystick(EXPANDER_ADRESS), 6);
      color_cell(index, ILI9341_RED);

      delay(10);
    }
}
