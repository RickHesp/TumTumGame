#include <avr/io.h>
#include <util/delay.h>
#include "countdown.h"
#include "nunchuck.h"
#include "nunchuckdraw.h"
#include "TWI.h"

#define BAUD 9600
#define MYUBRR ((F_CPU / (16UL * BAUD)) - 1)
#define NUNCHUCK_ADDR 0x52
#define EXPANDER_ADDR 0x21

// // ---------- UART ----------
// class UART {
// public:
//     static void init(unsigned int ubrr) {
//         UBRR0H = static_cast<unsigned char>(ubrr >> 8);
//         UBRR0L = static_cast<unsigned char>(ubrr);
//         UCSR0B = (1 << TXEN0);                        // enable TX
//         UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);       // 8N1
//     }

//     static void transmit(unsigned char data) {
//         while (!(UCSR0A & (1 << UDRE0)));
//         UDR0 = data;
//     }

//     static void printString(const std::string& str) {
//         for (char c : str) {
//             transmit(static_cast<unsigned char>(c));
//         }
//     }

//     static void printByte(uint8_t val) {
//         char buf[4];
//         itoa(val, buf, 10);
//         printString(buf);
//     }
// };

int main() {
    init();
    uint8_t data[2];
    int8_t digit = 0;

    TWI_init();
    nunchuck_init(NUNCHUCK_ADDR);

    while (1) {
        countDown_step(EXPANDER_ADDR, digit);
        digit = (digit + 1) % 10;

        NunchuckJoystick js = nunchuck_readJoystick(NUNCHUCK_ADDR);
        data[0] = js.x;
        data[1] = js.y;
        draw_joystick(js);


        _delay_ms(100);
    }
}
