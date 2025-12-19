#include <avr/io.h>
#include <util/delay.h>

#ifdef __cplusplus
extern "C" {
#endif

void TWI_init(void);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8_t data);
uint8_t TWI_read_ACK();
uint8_t TWI_read_NACK();

#ifdef __cplusplus
}
#endif