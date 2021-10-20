#ifndef MECH423_LIB_TIMER_H
#define MECH423_LIB_TIMER_H

#include <stdlib.h>
/*
Capabilities:

[1] Configure a square wave to output on a pin. Specify the frequency of the clock and of the square wave; this function calculates the necessary dividers.
- Uses one timer, and outputs square waves on both outputs with different duty cycles
- Automatically sets up the timer
- Does not setup the output pins; you need to change PxSEL0, PxSEL1, PxDIR


[2] Configure a function to run at periodic intervals. Specify the frequency of the clock and of your function; dividers are automatically calculated.
- Uses one timer
- Automatically sets up timer
- Interrupts are not globally enabled; call _EINT() before entering infinite loop


[3] Configure capture pins
- ISRs are configured for you, you just need to supply a function pointer which does some action.
- Still need to have a separate call to initialize the timer to an appropriate frequency.
- Does not setup the input pins; you need to change PxSEL0, PxSEL1, PxDIR

Process:
// Write a function
void doStuff(){
    unsigned int = TA0CCR1;
    ...
}

// Initialize a timer
Timer_init(TimerA0, ...);

// Set the interrupt function for the CCR
TimerA0_onInterrupt1 = &doStuff;

// Configure the capture register
Timer_configureCapture(...)

[4] More convenient ISRs
ISRs are converted into global function pointer variables;
these are listed below:
*/
// Timer A0
void (*TimerA0_onInterrupt0)(void) = NULL; // Corresponds to CCR0
void (*TimerA0_onInterrupt1)(void) = NULL; // CCR1
void (*TimerA0_onInterrupt2)(void) = NULL; // CCR2
void (*TimerA0_onOverflow)(void) = NULL; // When timer A overflows
void (*TimerA1_onInterrupt0)(void) = NULL;
void (*TimerA1_onInterrupt1)(void) = NULL;
void (*TimerA1_onInterrupt2)(void) = NULL;
void (*TimerA1_onOverflow)(void) = NULL;
void (*TimerB0_onInterrupt0)(void) = NULL;
void (*TimerB0_onInterrupt1)(void) = NULL;
void (*TimerB0_onInterrupt2)(void) = NULL;
void (*TimerB0_onOverflow)(void) = NULL;
void (*TimerB1_onInterrupt0)(void) = NULL;
void (*TimerB1_onInterrupt1)(void) = NULL;
void (*TimerB1_onInterrupt2)(void) = NULL;
void (*TimerB1_onOverflow)(void) = NULL;
void (*TimerB2_onInterrupt0)(void) = NULL;
void (*TimerB2_onInterrupt1)(void) = NULL;
void (*TimerB2_onInterrupt2)(void) = NULL;
void (*TimerB2_onOverflow)(void) = NULL;
/*
Set these to NULL to disable them. Set them to your own function to have them run on ISRs. If you do so, you may have to manually enable some interrupts.
*/

void (**Timer_interrupt_pointers[])(void) =
{
    &TimerA0_onInterrupt0,
    &TimerA0_onInterrupt1,
    &TimerA0_onInterrupt2,
    &TimerA0_onOverflow,
    &TimerA1_onInterrupt0,
    &TimerA1_onInterrupt1,
    &TimerA1_onInterrupt2,
    &TimerA1_onOverflow,
    &TimerB0_onInterrupt0,
    &TimerB0_onInterrupt1,
    &TimerB0_onInterrupt2,
    &TimerB0_onOverflow,
    &TimerB1_onInterrupt0,
    &TimerB1_onInterrupt1,
    &TimerB1_onInterrupt2,
    &TimerB1_onOverflow,
    &TimerB2_onInterrupt0,
    &TimerB2_onInterrupt1,
    &TimerB2_onInterrupt2,
    &TimerB2_onOverflow};

typedef enum {
    TimerA0 = 0,
    TimerA1,
    TimerB0,
    TimerB1,
    TimerB2
} Timer;

#define TIMER_RW_FUNCTION(reg) \
void Timer_write##reg##_(Timer timer, unsigned int val) { \
    switch(timer) {                      \
    case TimerA0: TA0##reg = val; break; \
    case TimerA1: TA1##reg = val; break; \
    case TimerB0: TB0##reg = val; break; \
    case TimerB1: TB1##reg = val; break; \
    case TimerB2: TB2##reg = val; break; \
    }                                    \
} \
unsigned int Timer_read##reg##_(Timer timer) { \
    switch(timer) {                 \
    case TimerA0: return TA0##reg;  \
    case TimerA1: return TA1##reg;  \
    case TimerB0: return TB0##reg;  \
    case TimerB1: return TB1##reg;  \
    case TimerB2: return TB2##reg;  \
    }                               \
    return 0; \
}

// Define registers
TIMER_RW_FUNCTION(CTL)
TIMER_RW_FUNCTION(R)
TIMER_RW_FUNCTION(CCTL0)
TIMER_RW_FUNCTION(CCTL1)
TIMER_RW_FUNCTION(CCTL2)
TIMER_RW_FUNCTION(CCR0)
TIMER_RW_FUNCTION(CCR1)
TIMER_RW_FUNCTION(CCR2)
TIMER_RW_FUNCTION(IV)
TIMER_RW_FUNCTION(EX0)

// Define all the ISRs
#define TIMER_CALL_IF_EXISTS(fcn) if(fcn != NULL) {fcn();}

// Timer A0
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TimerA0Ccr0Isr(){
    TIMER_CALL_IF_EXISTS(TimerA0_onInterrupt0);
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TimerA0IvIsr(){
    switch(__even_in_range(TA0IV, 0x0E)) {
    case 0: break; // nothing
    case 2: TIMER_CALL_IF_EXISTS(TimerA0_onInterrupt1); break;
    case 4: TIMER_CALL_IF_EXISTS(TimerA0_onInterrupt2); break;
    case 6: break; // reserved
    case 8: break; // reserved
    case 0xA: break; // reserved
    case 0xC: break; // reserved
    case 0xE: TIMER_CALL_IF_EXISTS(TimerA0_onOverflow); break;
    }
}

// Timer A1
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TimerA1Ccr0Isr(){
    TIMER_CALL_IF_EXISTS(TimerA1_onInterrupt0);
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void TimerA1IvIsr(){
    switch(__even_in_range(TA1IV, 0x0E)) {
    case 0: break; // nothing
    case 2: TIMER_CALL_IF_EXISTS(TimerA1_onInterrupt1); break;
    case 4: TIMER_CALL_IF_EXISTS(TimerA1_onInterrupt2); break;
    case 6: break; // reserved
    case 8: break; // reserved
    case 0xA: break; // reserved
    case 0xC: break; // reserved
    case 0xE: TIMER_CALL_IF_EXISTS(TimerA1_onOverflow); break;
    }
}

// Timer B0
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TimerB0Ccr0Isr(){
    TIMER_CALL_IF_EXISTS(TimerB0_onInterrupt0);
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TimerB0IvIsr(){
    switch(__even_in_range(TB0IV, 0x0E)) {
    case 0: break; // nothing
    case 2: TIMER_CALL_IF_EXISTS(TimerB0_onInterrupt1); break;
    case 4: TIMER_CALL_IF_EXISTS(TimerB0_onInterrupt2); break;
    case 6: break; // reserved
    case 8: break; // reserved
    case 0xA: break; // reserved
    case 0xC: break; // reserved
    case 0xE: TIMER_CALL_IF_EXISTS(TimerB0_onOverflow); break;
    }
}

// Timer B1
#pragma vector=TIMER1_B0_VECTOR
__interrupt void TimerB1Ccr0Isr(){
    TIMER_CALL_IF_EXISTS(TimerB1_onInterrupt0);
}

#pragma vector=TIMER1_B1_VECTOR
__interrupt void TimerB1IvIsr(){
    switch(__even_in_range(TB1IV, 0x0E)) {
    case 0: break; // nothing
    case 2: TIMER_CALL_IF_EXISTS(TimerB1_onInterrupt1); break;
    case 4: TIMER_CALL_IF_EXISTS(TimerB1_onInterrupt2); break;
    case 6: break; // reserved
    case 8: break; // reserved
    case 0xA: break; // reserved
    case 0xC: break; // reserved
    case 0xE: TIMER_CALL_IF_EXISTS(TimerB1_onOverflow); break;
    }
}

// Timer B2
#pragma vector=TIMER2_B0_VECTOR
__interrupt void TimerB2Ccr0Isr(){
    TIMER_CALL_IF_EXISTS(TimerB2_onInterrupt0);
}

#pragma vector=TIMER2_B1_VECTOR
__interrupt void TimerB2IvIsr(){
    switch(__even_in_range(TB2IV, 0x0E)) {
    case 0: break; // nothing
    case 2: TIMER_CALL_IF_EXISTS(TimerB2_onInterrupt1); break;
    case 4: TIMER_CALL_IF_EXISTS(TimerB2_onInterrupt2); break;
    case 6: break; // reserved
    case 8: break; // reserved
    case 0xA: break; // reserved
    case 0xC: break; // reserved
    case 0xE: TIMER_CALL_IF_EXISTS(TimerB2_onOverflow); break;
    }
}

/**
 * @brief Stops the timer. A lot of configuration data is cleared as a result. To reset the timer, call Timer_init
 * @param id Timer to stop.
 **/
void Timer_stop(Timer id) {
    Timer_writeCTL_(id, 0);
}

/**
 * @brief Sets up a timer
 * @param clockSource: Either TASSEL__ACLK or TASSEL__SMCLK
 * @param divider: One of ID__1, ID__2, ID__4, ID__8.
 *    Number indicates how much the source clock is divided by.
 * @param extraDivider: Integer in [1, 8] indicating additional factor of
 *          clock division
 * @param mode: One of MC__STOP, MC__UP, MC__CONTINUOUS, MC__UPDOWN.
 *    STOP stops the timer.
 *    UP counts up to capture/compare register 0 before wrapping
 *    CONTINUOUS counts up to 0xFFFF before wrapping
 *    UPDOWN counts up to capture/compare register 0 then back down to 0
 * */
void Timer_init(
        Timer id,
        unsigned int clockSource,
        unsigned int divider,
        unsigned int extraDivider,
        unsigned int mode
        ){
    Timer_writeCTL_(id,clockSource + divider + mode);
    if(extraDivider > 8) {
        extraDivider = 8;
    } else if (extraDivider == 0) {
        extraDivider = 1;
    }
    Timer_writeEX0_(id,extraDivider - 1);
}


/**
 * @brief Configure a timer in capture mode.
 * @param id Which timer to configure this for
 * @param n Which compare/capture register to use; affects input pin. Can be 0, 1 or 2.
 * @param edgeMode - any of:
 *      CM_0 - disabled
 *      CM_1 - capture on rising edge
 *      CM_2 - capture on falling edge
 *      CM_3 - capture on both edges
 * @param inputSelect - either CCIS_0 for CCIxA or CCIS_1 for CCIxB. Check the MSP Datasheet, section 6.11 - these have tables indicating whether a particular pin is CCIxA or CCIxB.
 * @param function - your ISR (but just a regular function). The ISR function needs to read TAxCCRn or TBxCCRn, which holds the captured timer value.
 *
 * Before calling this function, initialize the timer with Timer_init.
 * The most logical mode to use Timer_init in is MC__CONTINUOUS.
 * */
void Timer_configureCapture(Timer id, int n, unsigned int edgeMode, unsigned char inputSelect, void (*function)(void)) {
    unsigned int cctln = edgeMode + inputSelect
            + CAP // capture mode
            + CCIE; // enable interrupts

    switch(n){
    case 0: Timer_writeCCTL0_(id, cctln); break;
    case 1: Timer_writeCCTL1_(id, cctln); break;
    case 2: Timer_writeCCTL2_(id, cctln); break;
    }

    *(Timer_interrupt_pointers[4 * id + n]) = function;
}

/**
 * @brief Compute what dividers to use, for maximum resolution
 * @param clockFrequency Source clock frequency
 * @param targetFrequency Target frequency
 * @param divider Output variable indicating input divider value (ID__*)
 * @param extraDivider Output variable indicating extra divider value ([1, 8])
 * @return Resulting timer frequency, after division
 **/
unsigned long Timer_computeDividers_(unsigned long clockFrequency, unsigned long targetFrequency, unsigned int* divider, unsigned int* extraDivider) {
    // Coarse divider
    if (targetFrequency > (clockFrequency / 8 / 65535 + 1)) {
        // divide by 8 representing the extra divider
        *divider = ID__1;
    } else if (targetFrequency > (clockFrequency / 2 / 8 / 65535 + 1)) {
        *divider = ID__2;
        clockFrequency /= 2;
    } else if (targetFrequency > (clockFrequency / 4 / 8 / 65535 + 1)) {
        *divider = ID__4;
        clockFrequency /= 4;
    } else if (targetFrequency > (clockFrequency / 8 / 8 / 65535 + 1)) {
        *divider = ID__8;
        clockFrequency /= 8;
    } else {
        // uh oh, we can't make this square wave... try our best though
        targetFrequency = clockFrequency / 8 / 8 / 65535 + 2;
        *divider = ID__8;
        clockFrequency /= 8;
    }

    // Fine divider
    int exDiv;
    for (exDiv = 1; exDiv <= 8; exDiv++) {
        unsigned long afterDiv = clockFrequency / exDiv;
        if (targetFrequency > afterDiv / 65535 + 1) {
            clockFrequency = afterDiv;
            *extraDivider = exDiv;
            break;
        }
    }

    return clockFrequency;
}

/**
 * @brief Output a square wave using a timer.
 * @param id Which timer to configure this for
 * @param clockSource Either TASSEL__ACLK or TASSEL__SMCLK
 * @param clockFrequency Frequency of the clock source (Hz), e.g. SMCLK or ACLK.
 * @param waveFrequency Frequency of the square wave (Hz).
 * @param dutyCycle1 Number in [0, 100] indicating duty cycle for pin 1 in %age.
 * @param dutyCycle2 Number in [0, 100] indicating duty cycle for pin 2.
 *
 * Output generated on TAx.n or TBx.n pin, where n is 1 for dutyCycle1 and 2 for dutyCycle2.
 * Don't forget to configure PyDIR, PySEL1, and PySEL0 for those pins.
 *
 * If duty cycle pins are unused, pass in -1.
 */
void Timer_configureSquareWave(Timer id, unsigned int clockSource, unsigned long clockFrequency, unsigned long waveFrequency, int dutyCycle1, int dutyCycle2) {
    /* Step 1 - figure out divider to use.
    The smallest frequency you can get is CLK / 65535, by setting the CCR value to its maximum.
    Start with the coarse divider
    */
    unsigned int divider;
    unsigned int extraDivider;
    clockFrequency = Timer_computeDividers_(clockFrequency, waveFrequency, &divider, &extraDivider);

    // Initialize the clock
    Timer_init(id, clockSource, divider, extraDivider, MC__UP);

    /* Step 2 - figure out what to put in CCR0 */
    Timer_writeCCR0_(id, clockFrequency / waveFrequency);

    /* Step 3 - Figure out what to put in CCRn*/
    if (dutyCycle1 != -1) {
        Timer_writeCCR1_(id, clockFrequency / waveFrequency * dutyCycle1 / 100);
        Timer_writeCCTL1_(id, OUTMOD_7);
    }
    if (dutyCycle2 != -1) {
        Timer_writeCCR2_(id,clockFrequency / waveFrequency * dutyCycle2 / 100);
        Timer_writeCCTL2_(id, OUTMOD_7);
    }
}

/**
 * @brief Configure a timer to cause an interrupt with a defined frequency.
 * @param clockSource Either TASSEL__ACLK or TASSEL__SMCLK
 * @param clockFrequency Frequency of the clock source (Hz), e.g. SMCLK or ACLK.
 * @param interruptFrequency Frequency of the interrupt (Hz).
 * @param function The function you want to run at [xxx] frequency.
 **/
void Timer_configurePeriodicInterrupt(Timer id, unsigned int clockSource, unsigned long clockFrequency, unsigned long interruptFrequency, void (*function)(void)) {

    // Determine clock dividers
    unsigned int divider, extraDivider;
    clockFrequency = Timer_computeDividers_(clockFrequency, interruptFrequency, &divider, &extraDivider);

    // Initialize the clock
    Timer_init(id, clockSource, divider, extraDivider, MC__UP);

    // Configure CCR0 to set frequency of overflow
    Timer_writeCCR0_(id, clockFrequency / interruptFrequency);

    // Configure CCR0 to interrupt on counting to the correct value
    switch(id) {
    case TimerA0: TimerA0_onInterrupt0 = function; break;
    case TimerA1: TimerA1_onInterrupt0 = function; break;
    case TimerB0: TimerB0_onInterrupt0 = function; break;
    case TimerB1: TimerB1_onInterrupt0 = function; break;
    case TimerB2: TimerB2_onInterrupt0 = function; break;
    }

    // Enable CCR0 interrupts
    Timer_writeCCTL0_(id, CCIE);
}

#endif
