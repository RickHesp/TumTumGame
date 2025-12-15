#ifndef SENDCOMMAND_H
#define SENDCOMMAND_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void send_command(uint8_t toggle, uint8_t address, uint8_t command);
void send_ack();
void await_ack();
void receive_acknowledgement();
void init_ir_sender();

extern bool wait_ack;


#ifdef __cplusplus
}
#endif
#endif