#include <brightness.h> 
#include "drawGrid.h"


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
