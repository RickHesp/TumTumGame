#include <brightness.h> 
#include "drawGrid.h"


int main(void){
    init();
    grid_init();
    brightness_init();
    while (1)
    {
      delay(1000);
    }
}
