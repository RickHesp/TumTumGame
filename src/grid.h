#ifndef GRID_H
#define GRID_H

#include <avr/io.h>

struct gridCell {
    uint8_t boat;//0=no boat, 1=boat
    uint8_t hit;//0=not hit, 1=hit
    uint8_t selected;//0=not selected, 1=selected
};

//declarations of own and opponent grids
extern gridCell own_grid[36];//own grid
extern gridCell opp_grid[36];//opponent grid

//initialize all cells in a grid
void initCells(gridCell *grid);

//==========functions to edit own grid==========//
bool isBoatAt(uint8_t cell);
void selectCell(uint8_t cell);
void deselectCell(uint8_t cell);
void placeBoat(uint8_t cell);
void hitCell(uint8_t cell);
//==========functions to edit opponent grid==========//
void oppHitCell(uint8_t cell);
void oppPlaceBoat(uint8_t cell);
void oppSelectCell(uint8_t cell);
void oppDeselectCell(uint8_t cell);


#endif