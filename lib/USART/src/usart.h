/*
 * USART C library
 */
#ifndef USART_H
#define USART_H

#ifdef __cplusplus
extern "C" {
#endif

/* prototypes */
void USART_Init();
void USART_putc(char c);
void USART_Init_RXinterupt();
void USART_Transmit(uint8_t b);
uint8_t USART_Receive(void);
void USART_Print(const char *s);
void USART_Print_U16(uint16_t value);
void USART_Print_U8(uint8_t value);

#ifdef __cplusplus
}
#endif

#endif
