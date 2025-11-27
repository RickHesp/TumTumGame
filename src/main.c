#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "countdown.h"
#include "nunchuck.h"


#define BAUD 9600
#define MYUBRR ((F_CPU / (16UL * BAUD)) - 1)
#define NUNCHUCK_ADDR 0x52
#define EXPANDER_ADDR 0x21




// ---------- UART ----------
void UART_init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << TXEN0);                        // enable TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);       // 8N1
}

void UART_transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void UART_printString(const char *str) {
    while (*str) {
        UART_transmit(*str++);
    }
}

void UART_printByte(uint8_t val) {
    char buf[4];
    itoa(val, buf, 10);
    UART_printString(buf);
}

int main(void) {
    uint8_t data[2];
    int8_t digit = 0;

    UART_init(MYUBRR);
    TWI_init();
    nunchuck_init(NUNCHUCK_ADDR);

    while (1) {
        countDown_step(EXPANDER_ADDR, digit);
        digit = (digit + 1) % 10;

        data[0] = nunchuck_readJoystick(NUNCHUCK_ADDR).x;
        data[1] = nunchuck_readJoystick(NUNCHUCK_ADDR).y;

        UART_printString("JoyX: ");
        UART_printByte(data[0]);
        UART_printString("  JoyY: ");
        UART_printByte(data[1]);
        UART_printString("\r\n");

        _delay_ms(100);
    }
}

