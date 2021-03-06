#ifndef MECH423_LIB_CLOCK_H
#define MECH423_LIB_CLOCK_H


#include <msp430.h>
#include <stdbool.h>
#include <stdlib.h>



/**
 * @brief convert a divider value to the associated binary values needed
 * @param divVal: decimal divider value for a clock including 1, 2, 4, 8, 16, 32
 * @return binary value to get the appropriate  divider. If no appropriate divider value is provided then no divier will be applied.
 *
 * */
int Clock_dividerEncoder_(unsigned int divVal) {

    switch(divVal){
    case 1:
        return 0b000;
    case 2:
        return 0b001;
    case 4:
        return 0b010;
    case 8:
        return 0b011;
    case 16:
        return 0b100;
    case 32:
        return 0b101;
    }
    return 0;
}

typedef enum {
    Clock_DcoFreq_5, // 5.33
    Clock_DcoFreq_6, //6.77
    Clock_DcoFreq_8,
    Clock_DcoFreq_16,
    Clock_DcoFreq_20,
    Clock_DcoFreq_24
} Clock_DcoFreq; // MHz


/**
 * @brief Configure DCO at a given MHZ
 * @param Clock_DcoFreq: Integer value (only of the clock) Clock frequency can be given for 5.3, 6.7,8, 16,20, and 24.
 * @param mdiv: Clock divider to be applied to the main clock
 * @param smdiv: Clock divider to be applied to the SM clock
 * @param adiv: Clock divider to be applied to the A clock
 *
 *
 * Example: inputting 5 will set the frequency to 5.3 MHz
 * DCO set at 8 MHZ is the standard for all other peripherals
 *
 * Dividers can be any of 1, 2, 4, 8, 16, 32.
 *
 * */
void Clock_init(Clock_DcoFreq value, int mdiv, int smdiv, int adiv){
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

    CSCTL2 &= ~(SELA0 + SELA1 + SELA2 + SELM0 + SELM1 + SELM2 + SELS0 + SELS1 + SELS2); //reset all clock designation

    CSCTL2 |= SELA0 + SELA1 + SELM0 + SELM1 + SELS0 + SELS1; //set all clocks to DCO

    CSCTL3 = (Clock_dividerEncoder_(mdiv) << 0) + (Clock_dividerEncoder_(smdiv) << 4) + (Clock_dividerEncoder_(adiv) << 8); //setting the divider bits

}


#endif
