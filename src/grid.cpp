#include "grid.h"

gridCell own_grid[36];//own grid
gridCell opp_grid[36];//opponent grid

void initCells(gridCell *grid){
    for(uint8_t i=0; i<36;i++){
        grid[i].boat=0;
        grid[i].hit=0;
        grid[i].selected=0;
    }
}

static bool isCellInvalide(uint8_t cell){
    if(cell <0||cell >35) return true;
    return false;
}

//==========functions to edit own grid==========//
//check if boat is at cell(for placing boats)
bool isBoatAt(uint8_t cell){
    if(isCellInvalide(cell)) return false;
    if(own_grid[cell].boat==1) return true;
    else return false;
}

//select a cell
void selectCell(uint8_t cell){
    if(isCellInvalide(cell)) return;
    own_grid[cell].selected=1;//mark cell as selected
}

//deselect a cell
void deselectCell(uint8_t cell){
    if(isCellInvalide(cell)) return;
    own_grid[cell].selected=0;//mark cell as not selected
}
//place a boat at a cell
void placeBoat(uint8_t cell){
    if(isCellInvalide(cell)) return;
    own_grid[cell].boat=1;//place boat
}
//mark a cell as hit
void hitCell(uint8_t cell){
    if(isCellInvalide(cell)) return;
    own_grid[cell].hit=1;//mark cell as hit
}

//==========functions to edit opponent grid==========//
//mark a cell as hit on the opponent grid
void oppHitCell(uint8_t cell){
    if(isCellInvalide(cell)) return;
    opp_grid[cell].hit=1;//mark cell as hit
}
//place a boat at a cell on the opponent grid
void oppPlaceBoat(uint8_t cell){
    if(isCellInvalide(cell)) return;
    opp_grid[cell].boat=1;//place boat
}
//select a cell on the opponent grid
void oppSelectCell(uint8_t cell){
    if(isCellInvalide(cell)) return;
    opp_grid[cell].selected=1;//mark cell as selected
}
//deselect a cell on the opponent grid
void oppDeselectCell(uint8_t cell){
    if(isCellInvalide(cell)) return;
    opp_grid[cell].selected=0;//mark cell as not selected
}

