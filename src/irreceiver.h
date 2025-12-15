#ifndef IRRECEIVER_H
#define IRRECEIVER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

//init IR receiver
void init_ir_receiver(void);
int buffer_get(uint16_t *delta, uint8_t *state);

#ifdef __cplusplus
}
#endif
#endif 