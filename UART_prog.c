#include <stdint.h>
#include <stdbool.h>
#include "myInc/BIT_MATH.h"
#include "myInc/tm4c123gh6pm.h"
#include "myInc/GPIO_int.h"
#include "myInc/UART_INT.H"


void UART0_init(void)
{

    SYSCTL_RCGCUART_R |= 0x0001;
    SYSCTL_RCGCGPIO_R |= 0x0001;
    UART0_CTL_R &= ~0x0001;
    // 9600 baud rate at 16Mhz
    UART0_IBRD_R = 104;
    UART0_FBRD_R = 11;
    // 8-bit and 1 stop bit
    UART0_LCRH_R = 0x0070;
    UART0_CTL_R = 0x0301;
    // initiate portA pins 0-1
    GPIO_PORTA_AFSEL_R |= 0x03;
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) + 0x00000011;
    GPIO_PORTA_DEN_R |= 0x03;
    GPIO_PORTA_AMSEL_R &= ~0x03;

}
void UART0_sendchar (uint8_t data)
{
	//UARTCharPut(UART0_BASE, data);
    while(GET_BIT(UART0_FR_R,5));
    UART0_DR_R = data;
   
}
/* */
/* */
uint8_t UART0_ReceiveChar(void)
{
    while(GET_BIT(UART0_FR_R,4));
    uint8_t C = UART0_DR_R;
    UART0_sendchar(C);
    return C;

}
/* */
/* */
void UART0_SendString(uint8_t *str)
{
    while(*str != '\0')
    {
        UART0_sendchar(*str);
        str++;
    }
}
/*void Uart_ReceiveString(uint8_t *str)
{
    u8 i = 0;
    while(i < 8)
    {
        str[i] = Uart_ReceiveChar();
        i++;
    }
    str[i] = '\0';
}*/

