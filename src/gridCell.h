#include <avr/io.h>
#include <stdbool.h> 

struct gridCell {
    uint8_t boat;//0=no boat, 1=boat
    uint8_t hit;//0=not hit, 1=hit
    uint8_t selected;//0=not selected, 1=selected
};

gridCell grid[36];

void initCells(){
    for(uint8_t i=0; i<36;i++){
        grid[i].boat=0;
        grid[i].hit=0;
        grid[i].selected=0;
    }
}

//check if boat is at cell
bool isBoatAt(uint8_t cell){
    if(cell <0|| cell > 35) return false;
    if(grid[cell].boat==1) return true;
    else return false;
}

//select a cell
void selectCell(uint8_t cell){
    if(cell <0|| cell > 35) return;
    grid[cell].selected=1;//mark cell as selected
}

//deselect a cell
void deselectCell(uint8_t cell){
    if(cell <0|| cell > 35) return;
    grid[cell].selected=0;//mark cell as not selected
}
//place a boat at a cell
void placeBoat(uint8_t cell){
    if(cell <0||cell>35) return;
    grid[cell].boat=1;//place boat
}
//mark a cell as hit
void hitCell(uint8_t cell){
    if(cell <0||cell>35) return;
    grid[cell].hit=1;//mark cell as hit
}


