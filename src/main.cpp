#include <brightness.h> 
#include "drawGrid.h"
#include <Adafruit_ILI9341.h>
#include "nunchuck.h"
#include "TWI.h"


int main(void){
    init();
    grid_init();
    brightness_init();
    while (1)
    {
      delay(1000);
      color_cell(5, ILI9341_RED);
    }
}
