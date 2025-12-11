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

//draw the grid from gridcell array
void fill_grid(gridCell *grid);
#endif