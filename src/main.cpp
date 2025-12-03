#include "sendcommand.h"
#include "usart.h"

int main(void) {
    init_ir_sender();
    USART_Init();

    while(1) {
        //send IR command
        if(send_next_command_flag){
            send_next_command_flag = 0;
            send_command(0, 1, 0x65);
        }

        // read IR
        while(ir_tail != ir_head) {
            if(ir_buffer[ir_tail])
                USART_Transmit('1');
            else
                USART_Transmit('0');
            ir_tail = (ir_tail + 1) % BUFFER_SIZE;
        }
    }
}
