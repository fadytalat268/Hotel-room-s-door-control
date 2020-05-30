#ifndef _GPIO_INT_H
#define _GPIO_INT_H

#define PORT_A 0
#define PORT_B 1
#define PORT_C 2
#define PORT_D 3
#define PORT_E 4
#define PORT_F 5


#define PIN0     0
#define PIN1     1
#define PIN2     2
#define PIN3     3
#define PIN4     4
#define PIN5     5
#define PIN6     6
#define PIN7     7

#define ALL_PORT 8

#define DIGITAL_MODE      1     
#define ANALOG_MODE       2
#define ALTERNATIVE_MODE  3

#define INPUT  0
#define OUTPUT 1

#define LOW 0 
#define HIGH 1


void GPIO_Init(uint8_t PORTID, uint8_t PIN, uint8_t MODE);

void GPIO_SetPinDirection(uint8_t PORTID, uint8_t PIN, uint8_t DIR);
 
void GPIO_SetPortDirection(uint8_t PORTID, uint8_t DIR);

void GPIO_SetPinValue(uint8_t PORTID, uint8_t PIN, uint8_t VAL);

void GPIO_SetPortValue(uint8_t PORTID, uint8_t VAL);

uint8_t GPIO_GetPinValue(uint8_t PORTID, uint8_t PIN);



#endif


