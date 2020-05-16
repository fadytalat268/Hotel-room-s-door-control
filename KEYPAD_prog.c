/***************************************************************************************/
/****************************           KEYPAD_DRIVER          ******************************/
/***************************************************************************************/

#include <stdint.h>
#include "myInc/BIT_MATH.h"
#include "myInc/GPIO_int.h"
#include "myInc/KEYPAD_int.h"

#include <myInc/delay.h>


const volatile uint8_t keypad[16]={'1','2','3','A',
                          '4','5','6','B',
                          '7','8','9','C',
                          '*','0','#','D'};

void KEYPAD_Init(void)
{
    GPIO_Init(KeyPad_IN_PORT, PIN0, DIGITAL_MODE);
    GPIO_SetPinDirection(KeyPad_IN_PORT, PIN0, INPUT);
    GPIO_Init(KeyPad_IN_PORT, PIN1, DIGITAL_MODE);
    GPIO_SetPinDirection(KeyPad_IN_PORT, PIN1, INPUT);
    GPIO_Init(KeyPad_IN_PORT, PIN2, DIGITAL_MODE);
    GPIO_SetPinDirection(KeyPad_IN_PORT, PIN2, INPUT);
    GPIO_Init(KeyPad_IN_PORT, PIN3, DIGITAL_MODE);
    GPIO_SetPinDirection(KeyPad_IN_PORT, PIN3, INPUT);

    GPIO_Init(KeyPad_OUT_PORT, PIN4, DIGITAL_MODE);
    GPIO_SetPinDirection(KeyPad_OUT_PORT, PIN4, OUTPUT);
    GPIO_Init(KeyPad_OUT_PORT, PIN5, DIGITAL_MODE);
    GPIO_SetPinDirection(KeyPad_OUT_PORT, PIN5, OUTPUT);
    GPIO_Init(KeyPad_OUT_PORT, PIN6, DIGITAL_MODE);
    GPIO_SetPinDirection(KeyPad_OUT_PORT, PIN6, OUTPUT);
    GPIO_Init(KeyPad_OUT_PORT, PIN7, DIGITAL_MODE);
    GPIO_SetPinDirection(KeyPad_OUT_PORT, PIN7, OUTPUT);
}


//returns the value of pressed button
uint8_t GET_KEYPAD_VALUE(void)
{
    //MUST connect row/column 1 to pin 0/4
    //MUST connect row/column 2 to pin 1/5
    //MUST connect row/column 3 to pin 2/6
    //MUST connect row/column 4 to pin 3/7
    uint8_t i,j;
    for(i = 0; i < 4; i++)
    {
        GPIO_SetPinValue(KeyPad_OUT_PORT, PIN4, HIGH);
        GPIO_SetPinValue(KeyPad_OUT_PORT, PIN5, HIGH);
        GPIO_SetPinValue(KeyPad_OUT_PORT, PIN6, HIGH);
        GPIO_SetPinValue(KeyPad_OUT_PORT, PIN7, HIGH);

        GPIO_SetPinValue(KeyPad_OUT_PORT, i+4, LOW);

        for(j = 4; j < 8; j++)
        {
            if(!GPIO_GetPinValue(KeyPad_IN_PORT, j-4))
            {
                while(!GPIO_GetPinValue(KeyPad_IN_PORT, j-4)); //don't return until user removes his hand off the button

                _delay_ms(5); //to avoid debouncing

                uint8_t numb = ((j-4)*4)+i+1;

                return numb; // return values from 1 to 16
            }
        }
    }

    return 0xff;

}


uint8_t KEYPAD_PressedKey(void)
{
    uint16_t num = GET_KEYPAD_VALUE();
    if(num != 0xff)
    {
        num=num-1;
        return keypad[num];
    }

    return 0xff;
}








