#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <arduino.h>
#include "countdown.h"

#define EXPANDER_ADRESS 0x52
uint16_t lastmove = 0;

int main(void){
    init();
    

    while (1) {
        countDown_start(EXPANDER_ADRESS);
    }

    return 0;
}