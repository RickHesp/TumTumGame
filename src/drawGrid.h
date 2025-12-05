#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <gridCell.h>

#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8

#define grid_width 6
#define grid_height 6
#define grid_pixel_width 240
#define grid_pixel_height 240
#define cell_pixel_width (grid_pixel_width / grid_width)//40px per cell
#define cell_pixel_height (grid_pixel_height / grid_height)//40px per cell

static uint8_t x=0;
static uint8_t y=0;




Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);



void grid_init(){
    tft.begin();
    tft.setRotation(1); //320x240

    //make the play field a square (240x240)
    int w = 240; //240px
    int h = tft.height(); //240px

    int horizontal_val = w/grid_width;
    int vertical_val = h/grid_height;

    //start drawing (6x6 field)
    tft.fillScreen(ILI9341_BLUE);
    for (int i = 0; i <= w; i+=vertical_val) {//make vertical lines all h/6 apart
        tft.drawFastVLine(i, 0, h, ILI9341_BLACK);
    }
    
    for(int i = 0; i <= h; i+=horizontal_val){ //make horizontal lines all w/6 apart
        tft.drawFastHLine(0, i, w, ILI9341_BLACK);
    }
}

void get_cell_location(uint8_t cell){
    //calculate row and column
    uint8_t index = cell;
    uint8_t row =index / grid_width;
    uint8_t col =index % grid_height;

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
    //get cell location
    get_cell_location(cell);

    //control the location and size of the highlighted cell
    uint16_t Xpos = x + 4;//x location of cell + margin(to not overlap grid lines)
    uint16_t Ypos = y + 4;//y location of cell + margin(to not overlap grid lines)
    uint16_t width = cell_pixel_width - 8;//width of cell - margin(to not overlap grid lines)
    uint16_t height = cell_pixel_height - 8;//height of cell - margin(to not overlap grid lines)
    
    //bigger rect to highlight cell
    tft.drawRect(Xpos, Ypos, width, height, color);//outer rect
    tft.drawRect(Xpos+1, Ypos+1, width-2, height-2, color);//inner rect
}

void draw_cross(uint8_t cell, uint16_t color)
{
    //get cell location
    get_cell_location(cell);


    //line from bottom left corner to top right corner
    tft.drawLine(x,y,x+cell_pixel_width,y+cell_pixel_height,color);
    tft.drawLine(x,y+1,x+cell_pixel_width-1,y+cell_pixel_height, color);
    tft.drawLine(x+1,y,x+cell_pixel_width,y+cell_pixel_height-1,color);

    //line from bottom right corner to top left corner
    tft.drawLine(x+cell_pixel_width,y,x,y+cell_pixel_height,color);
    tft.drawLine(x+cell_pixel_width-1,y,x,y+cell_pixel_height-1,color);
    tft.drawLine(x+cell_pixel_width,y+1,x+1,y+cell_pixel_height,color);
}

void fill_grid(){
    for(uint8_t i=0; i<36; i++){
        if(own_grid[i].boat==1){
            color_cell(i, ILI9341_GREEN);
        }if(own_grid[i].hit==1){
            draw_cross(i, ILI9341_RED);
        }if(own_grid[i].selected==1){
            highlight_cell(i, ILI9341_BLACK);
        }
    }
}