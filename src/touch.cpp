#include <TouchScreen.h>
#include <Arduino.h>


#define XP A2
#define YP A3
#define XM 7
#define YM 6

#define MINPRESSURE 31.7
#define MAXPRESSURE 1000


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

bool screen_touched(){
    TSPoint p = ts.getPoint();
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
        return true;
    }
    return false;
}

int printtouchValues(){

    TSPoint p = ts.getPoint();
    return p.z;
}
