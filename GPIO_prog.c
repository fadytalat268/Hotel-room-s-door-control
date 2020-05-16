#include <stdint.h>
#include "myInc/BIT_MATH.h"

#include "myInc/GPIO_Int.h"
#include "myInc/tm4c123gh6pm.h"
#include "myInc/GPIO_reg.h"


/* array of gpio pull up resistors registers */
volatile uint32_t *GPIO_PullUpRegisters [6] = {GPIO_PORTA_PUR, GPIO_PORTB_PUR, GPIO_PORTC_PUR, GPIO_PORTD_PUR, GPIO_PORTE_PUR, GPIO_PORTF_PUR};

/* array of gpio lock registers */
volatile uint32_t *GPIO_LockRegisters [6] = {GPIO_PORTA_LOCK, GPIO_PORTB_LOCK, GPIO_PORTC_LOCK, GPIO_PORTD_LOCK, GPIO_PORTE_LOCK, GPIO_PORTF_LOCK};

/* array of gpio commit registers */
volatile uint32_t *GPIO_CommitRegisters [6] = {GPIO_PORTA_CR, GPIO_PORTB_CR, GPIO_PORTC_CR, GPIO_PORTD_CR, GPIO_PORTE_CR, GPIO_PORTF_CR};

/* array of gpio alternative functions registers */
volatile uint32_t *GPIO_AltFuncRegisters [6] = {GPIO_PORTA_AFSEL, GPIO_PORTB_AFSEL, GPIO_PORTC_AFSEL, GPIO_PORTD_AFSEL, GPIO_PORTE_AFSEL, GPIO_PORTF_AFSEL};

/* array of gpio digital enable registers */
volatile uint32_t *GPIO_DigitalEnableRegisters [6] = {GPIO_PORTA_DEN, GPIO_PORTB_DEN, GPIO_PORTC_DEN, GPIO_PORTD_DEN, GPIO_PORTE_DEN, GPIO_PORTF_DEN};

/* array of gpio analog mode registers */
volatile uint32_t *GPIO_AnalogModeRegisters [6] = {GPIO_PORTA_AMSEL, GPIO_PORTB_AMSEL, GPIO_PORTC_AMSEL, GPIO_PORTD_AMSEL, GPIO_PORTE_AMSEL, GPIO_PORTF_AMSEL};

/* array of gpio port control registers */
volatile uint32_t *GPIO_PortControlRegisters [6] = {GPIO_PORTA_PCTL, GPIO_PORTB_PCTL, GPIO_PORTC_PCTL, GPIO_PORTD_PCTL, GPIO_PORTE_PCTL, GPIO_PORTF_PCTL};

/* array of gpio data registers */
volatile uint32_t *GPIO_DataRegisters [6] = {GPIO_PORTA_DATA, GPIO_PORTB_DATA, GPIO_PORTC_DATA, GPIO_PORTD_DATA, GPIO_PORTE_DATA, GPIO_PORTF_DATA};

/* array of gpio direction registers */
volatile uint32_t *GPIO_DirRegisters [6] = {GPIO_PORTA_DIR, GPIO_PORTB_DIR, GPIO_PORTC_DIR, GPIO_PORTD_DIR, GPIO_PORTE_DIR, GPIO_PORTF_DIR};


uint8_t MaxPinNum(uint8_t PORTID)
{
    return (PORTID == PORT_E)? 5 : (PORTID == PORT_F)? 4 : 7;
}

void GPIO_Init(uint8_t PORTID, uint8_t PIN, uint8_t MODE)
{
    if((PORTID > PORT_F)||(PORTID < PORT_A))        return;

    if(((PIN > MaxPinNum(PORTID))||(PIN < 0)) && (PIN != ALL_PORT))     return;

    if((MODE > ALTERNATIVE_MODE)||(MODE < DIGITAL_MODE))        return;

    SET_BIT(SYSCTL_RCGCGPIO_R,PORTID);
    while(!GET_BIT(SYSCTL_PRGPIO_R ,PORTID));

    *GPIO_LockRegisters[PORTID] = 0x4C4F434B ; // unlock reg
    *GPIO_CommitRegisters[PORTID] = 0xFF ; //enable accessing to all pins
    *GPIO_PullUpRegisters[PORTID] = 0xFF ; // all pull up resistors

    if(MODE==DIGITAL_MODE)
    {
        if(PIN==ALL_PORT)
        {
            *GPIO_AltFuncRegisters[PORTID] = 0x00 ; //input-output
            *GPIO_DigitalEnableRegisters[PORTID] = 0xFF ; //all digital
            *GPIO_AnalogModeRegisters[PORTID] =0x00 ; //not analog
            *GPIO_PortControlRegisters[PORTID] = 0x00 ; //not PWM nor I2C
        }
        else
        {
            CLR_BIT(*GPIO_AltFuncRegisters[PORTID],PIN);  //input-output
            SET_BIT(*GPIO_DigitalEnableRegisters[PORTID],PIN); //digtal
            CLR_BIT(*GPIO_AnalogModeRegisters[PORTID],PIN); //not analog
            CLR_BIT(*GPIO_PortControlRegisters[PORTID],PIN); //not PWM nor I2C
        }
    }
    else if(MODE==ANALOG_MODE)
    {


    }
    else if(MODE==ALTERNATIVE_MODE)
    {

    }
}
/*_______________________________________________________________*/

void GPIO_SetPinDirection(uint8_t PORTID, uint8_t PIN, uint8_t DIR)
{
    if((PORTID > PORT_F)||(PORTID < PORT_A))        return;

    if((PIN > MaxPinNum(PORTID))||(PIN < 0))        return;

    if((DIR > OUTPUT)||(DIR < INPUT))       return;

    if(DIR == OUTPUT)
    {
        SET_BIT(*GPIO_DirRegisters[PORTID], PIN);
    }
    else
    {
        CLR_BIT(*GPIO_DirRegisters[PORTID], PIN);
    }
}
/*_______________________________________________________________*/

void GPIO_SetPortDirection(uint8_t PORTID, uint8_t DIR)
{
    if((PORTID > PORT_F)||(PORTID < PORT_A))        return;

    *GPIO_DirRegisters[PORTID] = DIR;
}
/*_______________________________________________________________*/

void GPIO_SetPinValue(uint8_t PORTID, uint8_t PIN, uint8_t VAL)
{
    if((PORTID > PORT_F)||(PORTID < PORT_A))        return;

    if((PIN > MaxPinNum(PORTID))||(PIN < 0))        return;

    if((VAL > HIGH)||(VAL < LOW))       return;

    if(VAL == HIGH)
    {
        SET_BIT(*GPIO_DataRegisters[PORTID], PIN);
    }
    else
    {
        CLR_BIT(*GPIO_DataRegisters[PORTID], PIN);
    }
}
/*_______________________________________________________________*/

void GPIO_SetPortValue(uint8_t PORTID, uint8_t VAL)
{
    if((PORTID > PORT_F)||(PORTID < PORT_A))        return;

    *GPIO_DataRegisters[PORTID] = VAL;
}
/*_______________________________________________________________*/

uint8_t GPIO_GetPinValue(uint8_t PORTID, uint8_t PIN)
{
    if((PORTID > PORT_F)||(PORTID < PORT_A))        return 0xff;

    if((PIN > MaxPinNum(PORTID))||(PIN < 0))        return 0xff;

    return GET_BIT(*GPIO_DataRegisters[PORTID],PIN);
}









