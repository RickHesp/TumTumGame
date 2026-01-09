#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "grid.h"
#include "display.h"
#include "micros_timer.h"

#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8

//edit these to change grid size
#define GRID_WIDTH 6//6 cells wide
#define GRID_HEIGHT 6//6 cells tall
#define grid_pixel_width 240
#define grid_pixel_height 240
#define cell_pixel_width (grid_pixel_width / GRID_WIDTH)//40px per cell
#define cell_pixel_height (grid_pixel_height / GRID_HEIGHT)//40px per cell

// Custom Military Colors for the Warship
#define C_SKY      0x0019 
#define C_SEA      0x001F  
#define C_HULL     0x8410  
#define C_DECK     0x4208 
#define C_HIGHLIGHT 0xBDF7

static uint8_t x=0;
static uint8_t y=0;

uint16_t lastmove = 0;

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

void grid_init(){
    tft.begin();
    tft.setRotation(1); //320x240

    //make the play field a square (240x240)
    int w = 240; //240px
    int h = tft.height(); //240px

    int horizontal_val = w/GRID_WIDTH;
    int vertical_val = h/GRID_HEIGHT;

    //start drawing (6x6 field)
    tft.fillRect(0, 0, 320, 240, ILI9341_NAVY);
    for (int i = 0; i <= w; i+=vertical_val) {//make vertical lines all h/6 apart
        tft.drawFastVLine(i, 0, h, ILI9341_BLACK);
    }
    
    for(int i = 0; i <= h; i+=horizontal_val){ //make horizontal lines all w/6 apart
        tft.drawFastHLine(0, i, w, ILI9341_BLACK);
    }
}

void update_grid(){
    if(micros_timer() - lastmove > 100){
        fill_grid(own_grid);
        lastmove = micros_timer();
    }
    // drawRadarGrid();
}

void update_opp_grid(){
    if(micros_timer() - lastmove > 100){
        fill_grid(opp_grid);
        lastmove = micros_timer();
    }
}

void get_cell_location(uint8_t cell){
    //calculate row and column
    uint8_t index = cell;
    uint8_t row =index / GRID_WIDTH;
    uint8_t col =index % GRID_WIDTH; // fixed: use GRID_WIDTH

    //calculate x and y position
    x = col * cell_pixel_width;
    y = row * cell_pixel_height;
}


//color whole cell
void color_cell(uint8_t cell, uint16_t color)
{
    //get cell location
    get_cell_location(cell);

    //fill 1 cell (slightly smaller so lines remain visible)
    tft.fillRect(x+1, y+1, cell_pixel_width-1, cell_pixel_height-1, color);
}

//highlight cell with a border
void highlight_cell(uint8_t cell, uint16_t color)
{
    
    get_cell_location(cell);

    //control the location and size of the highlighted cell
    uint16_t Xpos = x + 4;//x location of cell + margin(to not overlap grid lines)
    uint16_t Ypos = y + 4;//y location of cell + margin(to not overlap grid lines)
    uint16_t width = cell_pixel_width - 8;//width of cell - margin(to not overlap grid lines)
    uint16_t height = cell_pixel_height - 8;//height of cell - margin(to not overlap grid lines)
    
    //draw two rectangles to make a thicker border
    tft.drawRect(Xpos, Ypos, width, height, color);//the outer rectangle, shifted to not overlap the inner rect
    tft.drawRect(Xpos+1, Ypos+1, width-2, height-2, color);//the inner rectangle, shifted to not overlap with the outer rect
}

void draw_cross(uint8_t cell, uint16_t color)
{
    //get cell location
    get_cell_location(cell);


    //line from bottom left corner to top right corner, made thicker with 3 lines total
    tft.drawLine(x,y,x+cell_pixel_width,y+cell_pixel_height,color);
    tft.drawLine(x,y+1,x+cell_pixel_width-1,y+cell_pixel_height, color);//shifted slightly
    tft.drawLine(x+1,y,x+cell_pixel_width,y+cell_pixel_height-1,color);//shifted slightly

    //line from bottom right corner to top left corner, made thicker with 3 lines total
    tft.drawLine(x+cell_pixel_width,y,x,y+cell_pixel_height,color);
    tft.drawLine(x+cell_pixel_width-1,y,x,y+cell_pixel_height-1,color);//shifted slightly
    tft.drawLine(x+cell_pixel_width,y+1,x+1,y+cell_pixel_height,color);//shifted slightly
}


void fill_grid(gridCell *grid){
    for(uint8_t i=0; i<36; i++){
        if(grid[i].boat==1){
            drawBoat(i);
        }if(grid[i].hit==1){
            draw_cross(i, ILI9341_RED);
        }if(grid[i].selected==1){
            highlight_cell(i, ILI9341_BLACK);
        }if(grid[i].boat==0 && grid[i].hit==0 && grid[i].selected==0){
            color_cell(i, ILI9341_NAVY);
        }
    }
}

void drawButton(){
    tft.begin();
    tft.setRotation(1); //320x240
    tft.fillRoundRect(250, 50, 60, 40, 10, ILI9341_WHITE);
    tft.setCursor(260, 60);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.print("Start");
}

void Startscreen_init(){
    tft.begin();
    tft.setRotation(1); // 320x240
    
    
    drawWarshipArt();

    // Title
    tft.setTextSize(3);
    
    
    tft.setCursor(72, 32);
    tft.setTextColor(ILI9341_BLACK);
    tft.print("BATTLESHIP");
    
    
    tft.setCursor(70, 30);
    tft.setTextColor(ILI9341_WHITE);
    tft.print("BATTLESHIP");

    // "Press to Start" Prompt
    tft.setTextSize(2);
    tft.setCursor(80, 200);
    tft.setTextColor(ILI9341_YELLOW); 
    tft.print("TAP TO START");
}

void timer_init(int timer){
    tft.fillRect(265, 10, 50, 30, ILI9341_NAVY);
    //red if lower then 10 seconds left
    if(timer <= 10){
        tft.setTextColor(ILI9341_RED, ILI9341_NAVY);
    }else{
        tft.setTextColor(ILI9341_GREEN, ILI9341_NAVY);
    }
    tft.setCursor(265, 10);
    tft.setTextSize(3);
    tft.print(timer);
}

void drawYourTurn(int on_off){
    if(on_off){
        tft.setCursor(265, 50);
        tft.setTextSize(5);
        tft.setTextColor(ILI9341_GREEN, ILI9341_NAVY);
        tft.print("*");
        return;
    }else{
        tft.setCursor(265, 50);
        tft.setTextSize(5);
        tft.setTextColor(ILI9341_NAVY, ILI9341_NAVY);
        tft.print("*");
        return;
    }
}

void drawBoat(uint8_t gridNumber)
{
    uint8_t cell = gridNumber;
    get_cell_location(cell);

    uint16_t shipColor = ILI9341_DARKGREY;
    int cx = x + cell_pixel_width / 2;

    int boatHalfWidth = (cell_pixel_width * 0.4) / 2; 

    int topMargin = cell_pixel_height * 0.05; // 5% padding top
    int noseHeight = cell_pixel_height * 0.15; // Nose is 15% of height

    tft.fillRect(
        cx - boatHalfWidth,
        y + topMargin + noseHeight,
        boatHalfWidth * 2,
        cell_pixel_height - (topMargin + noseHeight + 5), // +5 for bottom margin
        shipColor
    );

    tft.fillTriangle(
        cx - boatHalfWidth, y + topMargin + noseHeight, // Bottom Left
        cx + boatHalfWidth, y + topMargin + noseHeight, // Bottom Right
        cx,                 y + topMargin,              // Top Tip
        shipColor
    );

    int sternHeight = 6;
    tft.fillRect(
        cx - (boatHalfWidth + 2), // Slightly wider
        y + cell_pixel_height - (sternHeight + 5),
        (boatHalfWidth * 2) + 4,
        sternHeight,
        shipColor
    );

    int detailRadius = boatHalfWidth / 3;
    
    tft.fillCircle(cx, y + (cell_pixel_height * 0.6), detailRadius, ILI9341_BLACK);
    tft.fillCircle(cx, y + (cell_pixel_height * 0.8), detailRadius, ILI9341_BLACK);
}

void drawWarshipArt() {
    tft.fillRect(0, 0, 320, 150, C_SKY);   // Sky
    tft.fillRect(0, 150, 320, 90, C_SEA);  // Sea

    // --- THE SHIP ---
    int shipY = 120;

    tft.fillTriangle(30, 150, 80, 150, 80, 120, C_HULL);

    tft.fillRect(80, 120, 200, 30, C_HULL);

    tft.fillRect(280, 120, 20, 30, C_HULL);

    tft.fillRect(45, 148, 255, 2, ILI9341_BLACK);


    tft.fillRect(130, 95, 60, 25, C_DECK);

    tft.fillRect(140, 80, 30, 15, C_HULL);
    tft.fillRect(142, 82, 26, 4, ILI9341_BLACK);

    tft.drawFastVLine(155, 50, 30, C_HIGHLIGHT);
    tft.drawFastHLine(145, 60, 20, C_HIGHLIGHT);
    tft.drawCircle(155, 50, 4, C_HIGHLIGHT); 


    tft.fillRect(90, 112, 20, 8, C_DECK);
    tft.fillRect(110, 114, 15, 3, C_HIGHLIGHT);
    
    // Rear Gun (Turret Y)
    tft.fillRect(220, 112, 20, 8, C_DECK);
    tft.fillRect(200, 114, 20, 3, C_HIGHLIGHT);

    // Railings or texture lines on hull
    tft.drawFastHLine(80, 125, 180, C_DECK);
}


void endscreen_init(bool won) {
    tft.setRotation(1);
    
    if (won) {
        // --- VICTORY SCREEN ---
        tft.fillScreen(ILI9341_NAVY);
        
        // Draw some "celebration" rays
        for(int i=0; i<320; i+=20) {
            tft.drawLine(160, 120, i, 0, ILI9341_YELLOW);
            tft.drawLine(160, 120, i, 240, ILI9341_YELLOW);
        }

        // Draw a central "Medal" circle
        tft.fillCircle(160, 110, 50, 0xFD20); // Gold
        tft.drawCircle(160, 110, 52, ILI9341_WHITE);
        
        // 3D Shadow Text
        tft.setTextSize(4);
        tft.setTextColor(ILI9341_BLACK);
        tft.setCursor(63, 173);
        tft.print("VICTORY!"); 
        
        tft.setTextColor(ILI9341_WHITE);
        tft.setCursor(60, 170);
        tft.print("VICTORY!");
        
        tft.setTextSize(2);
        tft.setCursor(75, 210);
        tft.print("The ocean is safe.");

    } else {
        // --- DEFEAT SCREEN ---
        // Dark gradient floor (simulating deep water)
        tft.fillScreen(ILI9341_BLACK);
        tft.fillRect(0, 120, 320, 120, 0x0008); // Dark grey/blue
        
        // Draw a "sinking" ship silhouette
        tft.fillTriangle(100, 140, 220, 140, 140, 100, 0x4208); // Sinking hull
        
        // Red glow from top
        for(int i=0; i<5; i++) {
            tft.drawRect(i, i, 320-(i*2), 240-(i*2), ILI9341_MAROON);
        }

        // 3D Shadow Text
        tft.setTextSize(4);
        tft.setTextColor(ILI9341_BLACK);
        tft.setCursor(53, 63);
        tft.print("DEFEATED");
        
        tft.setTextColor(ILI9341_RED);
        tft.setCursor(50, 60);
        tft.print("DEFEATED");

        tft.setTextSize(2);
        tft.setTextColor(ILI9341_LIGHTGREY);
        tft.setCursor(70, 200);
        tft.print("Your fleet has sunk...");
    }
}

void dropBomb(uint8_t gridNumber) {
    //Get Target coordinates
    get_cell_location(gridNumber); 
    int cx = x + (cell_pixel_width / 2);
    int cy = y + (cell_pixel_height / 2);

    tft.fillCircle(cx, cy, 10, ILI9341_WHITE);
    
    // 3. EXPLOSION EFFECT
    int maxRadius = (cell_pixel_width / 2) - 2;

    for (int r = 2; r <= maxRadius; r += 3) {
        // Inner "Core" of explosion
        tft.fillCircle(cx, cy, r, ILI9341_YELLOW);
        
        // Outer "Ring"
        tft.drawCircle(cx, cy, r + 1, ILI9341_ORANGE);
        tft.drawCircle(cx, cy, r + 2, ILI9341_RED);
        
       micros_ms_timer_delay(50000); // Short delay for animation effect
    }

    micros_ms_timer_delay(50000);
    color_cell(gridNumber, ILI9341_NAVY); // Reset cell background
}

