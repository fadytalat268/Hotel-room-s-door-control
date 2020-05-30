#ifndef _UART_INT_H
#define _UART_INT_H



void UART0_init(void);
void UART0_sendchar (uint8_t data);
uint8_t UART0_ReceiveChar(void);
void UART0_SendString(uint8_t *str);

#endif
