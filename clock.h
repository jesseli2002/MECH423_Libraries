#ifndef MECH423_LIB_CLOCK_H
#define MECH423_LIB_CLOCK_H


#include <msp430.h> 
#include <stdbool.h>
#include <stdlib.h>



/**
 * main.c
 */
int main(void)
{

	
	return 0;
}

typedef enum {
    Clock_DcoFreq_5, // 5.33
    Clock_DcoFreq_6,
    Clock_DcoFreq_8,
    Clock_DcoFreq_16,
    Clock_DcoFreq_20,
    Clock_DcoFreq_24
} Clock_DcoFreq;

/**
 * @brief Configure DCO at a given MHZ
 * @param Integer value (only of the clock) Clock frequency can be given for 5.3, 6.7,8, 16,20, and 24.
 *
 * Example: inputting 5 will set the frequency to 5.3 MHz
 * DCO set at 8 MHZ is the standard for all other peripherals
 *
 * */
void Clock_Init(Clock_DcoFreq value, int mdiv, int smdiv, int adiv){
    //Enter clock control password
    CSCTL0 = CSKEY;

    CSCTL1 &= ~(DCOFSEL0 + DCOFSEL1);

    switch(value){
    case Clock_DcoFreq_5:
        CSCTL1 &= ~(DCOFSEL0 + DCOFSEL1);
        CSCTL1 &= ~DCORSEL;
        break;
    case Clock_DcoFreq_6:
        CSCTL1 |= DCOFSEL0;
        CSCTL1 &= ~DCORSEL;
        break;
    case Clock_DcoFreq_8:
        CSCTL1 |= DCOFSEL1 + DCOFSEL0;
        CSCTL1 &= ~DCORSEL;
        break;
    case Clock_DcoFreq_16:
        CSCTL1 &= ~(DCOFSEL0 + DCOFSEL1);
        CSCTL1 |= DCORSEL;
        break;
    case Clock_DcoFreq_20:
        CSCTL1 |= DCOFSEL0;
        CSCTL1 |= DCORSEL;
        break;
    case Clock_DcoFreq_24:
        CSCTL1 |= DCOFSEL1 + DCOFSEL0;
        CSCTL1 |= DCORSEL;
        break;
    }

    CSCTL3 = Clock_dividerEncoder(mdiv) << 0 + Clock_dividerEncoder(smdiv) << 4 Clock_dividerEncoder(adiv) << 8;
}

/**
 * @brief set DCO to the desired clock
 * @param int of the clock ACLK = 1, MCLK = 2, SMCLK = 3
 * @param clock divider value you would want (1,2,4,8,16,32)
 *
 * */

void Clock_setDCOtoCLK(int CLKval, int CLKdiv){
    if(CLKval == 1){
        CSCTL2 |= SELA0 + SELA1;
    } else if (CLKval == 2){
        CSCTL2 |= SELM0 + SELM1;
    } else if (CLKval == 3){
        CSCTL2 |= SELS0 + SELS1;
    }
}

/**
 * @brief convert a divider value to the associated binary values needed
 * @param decimal divider value
 *
 * */
void  Clock_dividerEncoder(unsigned int divVal) {

    if(divVal == 1)
        return 0b000;
    if(divVal == 2)
        return 0b001;
    if(divVal == 4)
        return 0b010;
    if(divVal == 8)
        return 0b011;
    if(divVal == 16)
        return 0b100;
    if(divVal == 32)
        return 0b101;
}


