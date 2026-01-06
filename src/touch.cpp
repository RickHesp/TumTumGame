#include <Wire.h>
#include <Adafruit_FT6206.h>

#define XP A2
#define YP A3
#define XM 7
#define YM 6

#define MINPRESSURE 20
#define MAXPRESSURE 1000

Adafruit_FT6206 ts = Adafruit_FT6206();


bool screen_touched(){ 
    ts.begin(40);
    return ts.touched();    
}

