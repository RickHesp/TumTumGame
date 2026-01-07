#ifndef DISPLAY_H
#define DISPLAY_H

#include <avr/io.h>
#include "grid.h"

#define GRID_WIDTH 6//6 cells wide
#define GRID_HEIGHT 6//6 cells tall

//initialize the grid display
void grid_init();


//draw commands
void color_cell(uint8_t cell, uint16_t color);
void highlight_cell(uint8_t cell, uint16_t color);
void draw_cross(uint8_t cell, uint16_t color);
void drawButton();
void Startscreen_init();
void dropBomb(uint8_t gridNumber);

//draw the grid from gridcell array
void fill_grid(gridCell *grid);
void drawSeaBackground();
void drawRadarGrid();
void drawBoat(uint8_t gridNumber, uint8_t orientation);
void drawWarshipArt();


//endscreen
void endscreen_init(bool won);
void Startscreen_init();

//timer
void timer_init(int timer);
void drawYourTurn(int on_off);
void lives(int lives);

void update_grid();
#endif