#include "stdint.h"
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include <driverlib/interrupt.h>
#include "myInc/tm4c123gh6pm.h"
#include "myInc/BIT_MATH.h"
#include "myInc/delay.h"
#include "myInc/GPIO_int.h"
#include "myInc/KEYPAD_int.h"
#include "myInc/UART_INT.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
/* Macros */
#define NumberOFRooms 5
#define LOCK 0
#define FREE 1
#define OCCUPIED 2
#define CLEANING 3
#define Lock_Pin PIN2
#define Lock_Port PORT_D
#define Buzzer_Pin PIN3
#define Buzzer_Port PORT_D

/* Struct */
struct myStruct
{
    uint8_t Room_Status;
    uint8_t Door_Status;
    uint8_t Password[4];
}Default = {FREE, LOCK, "0000"};
typedef struct myStruct Room;

/* Prototypes */
void Rooms_Init(void);
void SysTick_Init(void);
void UnLock(void);
void Lock(void);
void Interrupt_PORTF_Init(void);
void Buzzer_On(void);
void Buzzer_Off(void);

/* Globals */
Room myRooms[NumberOFRooms];
uint8_t timer_counter = 0;

void GPIO_PORTF_Handler(void)
{
    if(!GET_BIT(GPIO_PORTF_DATA_R,4))
    {//check if PF4 caused the interrupt

        while(!GET_BIT(GPIO_PORTF_DATA_R,4));

        uint8_t i=0, Key;
        bool flag = true;
        SysTick_Init();
        Buzzer_On();
        while (timer_counter <5 && i<4)
        {

            Key=KEYPAD_PressedKey();
            if (Key!=0xFF)
            {
                if(Key != myRooms[2].Password[i])
                {
                    flag = false;
                    break;
                }
                i++;
            }
        }
        Buzzer_Off();
        if(timer_counter >= 5)
        {
            flag = false;
        }
        timer_counter = 0;
        NVIC_ST_CTRL_R=0x00000000;
        if (flag == true)
        {
            if(myRooms[2].Room_Status == OCCUPIED)
            {
                if(myRooms[2].Door_Status == LOCK)
                {
                    UnLock();
                    myRooms[2].Door_Status = !LOCK;
                }
                else
                {
                    Lock();
                    myRooms[2].Door_Status = LOCK;
                }
            }
        }

        GPIO_PORTF_ICR_R = 0x10;
    }
}


void GPIO_SysTick_Handler(void)
{
    timer_counter ++;
}

int main(void)
{
    UART0_init();
    Interrupt_PORTF_Init();
    GPIO_Init(Lock_Port, Lock_Pin, DIGITAL_MODE);
    GPIO_SetPinDirection(Lock_Port, Lock_Pin, OUTPUT);
    GPIO_Init(Buzzer_Port, Buzzer_Pin, DIGITAL_MODE);
    GPIO_SetPinDirection(Buzzer_Port, Buzzer_Pin, OUTPUT);
    Lock();
    Buzzer_Off();
    Rooms_Init();
    KEYPAD_Init();
    IntMasterEnable();
    while(1)
    {
        UART0_SendString("Enter Room Number (0 -> 4): ");
        uint8_t num = UART0_ReceiveChar() - '0';
        UART0_SendString("\r\n");
        if(num>=0 && num <5)
        {
            while(1)
            {
                UART0_SendString("Enter V to view Room or E to edit Room or Q to quit Room: ");
                uint8_t input = UART0_ReceiveChar();
                UART0_SendString("\r\n");
                if(input == 'V')
                {
                    UART0_SendString("Room Status is ");
                    if(myRooms[num].Room_Status == OCCUPIED)
                    {
                        UART0_SendString("Occupied\r\n");
                    }
                    else if(myRooms[num ].Room_Status == FREE)
                    {
                        UART0_SendString("Free\r\n");
                    }
                    else
                    {
                        UART0_SendString("Cleaning\r\n");
                    }
                    UART0_SendString("Password is: ");
                    UART0_sendchar(myRooms[num].Password[0]);
                    UART0_sendchar(myRooms[num].Password[1]);
                    UART0_sendchar(myRooms[num].Password[2]);
                    UART0_sendchar(myRooms[num].Password[3]);
                    UART0_SendString("\r\n");
                }
                else if(input == 'E')
                {
                    while(1)
                    {
                        UART0_SendString("Enter S to edit Room Status or P to edit Room Password or Q to quit Editing: ");
                        input = UART0_ReceiveChar();
                        UART0_SendString("\r\n");
                        if(input == 'S')
                        {
                            UART0_SendString("Enter O for Occupied or F for Free or C for Cleaning: ");
                            input = UART0_ReceiveChar();
                            UART0_SendString("\r\n");
                            if(input == 'O')
                            {
                                myRooms[num].Room_Status = OCCUPIED;
                                myRooms[num ].Door_Status = LOCK;
                                if(num == 2)
                                {
                                    Lock();
                                }
                            }
                            else if(input == 'F')
                            {
                                myRooms[num ].Room_Status = FREE;
                                myRooms[num ].Door_Status = LOCK;
                                if(num == 2)
                                {
                                    Lock();
                                }
                            }
                            else if(input == 'C')
                            {
                                myRooms[num ].Room_Status = CLEANING;
                                myRooms[num ].Door_Status = !LOCK;
                                if(num == 2)
                                {
                                    UnLock();
                                }
                            }
                            else
                            {
                                UART0_SendString("Invalid Input\r\n");
                            }
                        }
                        else if(input == 'P')
                        {
                            UART0_SendString("Enter New Password: ");
                            uint8_t j=0, key;
                            j=0;
                            while (j<4)
                            {
                                key=UART0_ReceiveChar();
                                myRooms[num].Password[j]=key;
                                j++;
                            }
                            UART0_SendString("\r\n");
                        }
                        else if(input == 'Q')
                        {
                            break;
                        }
                        else
                        {
                            UART0_SendString("Invalid Input\r\n");
                        }
                    }
                }
                else if(input == 'Q')
                {
                    break;
                }
                else
                {
                    UART0_SendString("Invalid Input\r\n");
                }
            }
        }

    }
    return 0;
}


void Rooms_Init(void)
{
    uint8_t i = 0 ;
    for(i = 0; i<NumberOFRooms; i++)
    {
        myRooms[i] = Default;
    }
}


void UnLock(void){
    GPIO_SetPinValue(Lock_Port, Lock_Pin, HIGH);
}


void Lock(void){
    GPIO_SetPinValue(Lock_Port, Lock_Pin, LOW);
}


void Buzzer_On(void){
    GPIO_SetPinValue(Buzzer_Port, Buzzer_Pin, HIGH);
}


void Buzzer_Off(void){
    GPIO_SetPinValue(Buzzer_Port, Buzzer_Pin, LOW);
}


void SysTick_Init(void)
{
    NVIC_ST_CTRL_R=0;
    NVIC_ST_RELOAD_R=0x00ffffff;
    NVIC_ST_CURRENT_R=0;
    NVIC_ST_CTRL_R=0x00000007;
    NVIC_ST_RELOAD_R=16000000-1;
    NVIC_ST_CURRENT_R=0;
}


void Interrupt_PORTF_Init(void)
{
    //initialize port F
    SYSCTL_RCGC2_R |= 0x20;
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0xff;
    GPIO_PORTF_AMSEL_R = 0x00;
    GPIO_PORTF_AFSEL_R = 0x00;
    GPIO_PORTF_PCTL_R = 0x00;
    GPIO_PORTF_DEN_R = 0x1f;
    GPIO_PORTF_DIR_R = 0X0E;
    GPIO_PORTF_PUR_R = 0x11;
    GPIO_PORTF_DATA_R &= ~0x0E;
    // interrupt config.
    GPIO_PORTF_IS_R &= ~0x10;
    GPIO_PORTF_IBE_R &= ~0x10;
    GPIO_PORTF_IEV_R &= ~0x10;
    GPIO_PORTF_ICR_R = 0x10;
    GPIO_PORTF_IM_R |= 0x10;
    NVIC_PRI7_R = (NVIC_PRI7_R  & 0xff00ffff) | 0x00400000;
    NVIC_EN0_R = 0x40000000;
}













































