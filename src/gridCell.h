#include <avr/io.h>
#include <stdbool.h> 

struct gridCell {
    uint8_t boat;//0=no boat, 1=boat
    uint8_t hit;//0=not hit, 1=hit
    uint8_t selected;//0=not selected, 1=selected
};

gridCell own_grid[36];//own grid
gridCell opp_grid[36];//opponent grid

void initCells(){
    for(uint8_t i=0; i<36;i++){
        own_grid[i].boat=0;
        own_grid[i].hit=0;
        own_grid[i].selected=0;
    }
}

//==========functions to edit own grid==========//
//check if boat is at cell
bool isBoatAt(uint8_t cell){
    if(cell <0|| cell > 35) return false;
    if(own_grid[cell].boat==1) return true;
    else return false;
}

//select a cell
void selectCell(uint8_t cell){
    if(cell <0|| cell > 35) return;
    own_grid[cell].selected=1;//mark cell as selected
}

//deselect a cell
void deselectCell(uint8_t cell){
    if(cell <0|| cell > 35) return;
    own_grid[cell].selected=0;//mark cell as not selected
}
//place a boat at a cell
void placeBoat(uint8_t cell){
    if(cell <0||cell>35) return;
    own_grid[cell].boat=1;//place boat
}
//mark a cell as hit
void hitCell(uint8_t cell){
    if(cell <0||cell>35) return;
    own_grid[cell].hit=1;//mark cell as hit
}

//==========functions to edit opponent grid==========//
//mark a cell as hit on the opponent grid
void oppHitCell(uint8_t cell){
    if(cell <0||cell>35) return;
    opp_grid[cell].hit=1;//mark cell as hit
}
//place a boat at a cell on the opponent grid
void oppPlaceBoat(uint8_t cell){
    if(cell <0||cell>35) return;
    opp_grid[cell].boat=1;//place boat
}
//select a cell on the opponent grid
void oppSelectCell(uint8_t cell){
    if(cell <0|| cell > 35) return;
    opp_grid[cell].selected=1;//mark cell as selected
}
//deselect a cell on the opponent grid
void oppDeselectCell(uint8_t cell){
    if(cell <0|| cell > 35) return;
    opp_grid[cell].selected=0;//mark cell as not selected
}

