#include <brightness.h> 
#include "drawGrid.h"



int main(void){
    init();//from arduino.h
    initCells();
    grid_init();
    brightness_init();
    while (1)
    {
      placeBoat(0);
      selectCell(0);
      hitCell(20);
      hitCell(35);
      placeBoat(20);
      fill_grid();
      delay(2000);
      deselectCell(0);

      
      fill_grid();
      delay(2000);
    }
}
