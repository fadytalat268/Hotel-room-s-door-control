#ifndef _KEYPAD_INT_H
#define _KEYPAD_INT_H


void KEYPAD_Init(void);


uint8_t GET_KEYPAD_VALUE(void);

uint8_t KEYPAD_PressedKey(void);

#define KeyPad_IN_PORT PORT_E
// pins 0,1,2,3 ROWS
#define KeyPad_OUT_PORT PORT_B
// pins 4,5,6,7 Columns

#endif
