#include <msp430.h>
#include <stdlib.h>

#include "led.h"
#include "timer.h"

void flashLed() {
    Led_toggle(0b0011);
}
void flashLed2() {
    Led_toggle(0b1100);
}

unsigned int time = 0;
unsigned int time2 = 0;

void getTime() {
    time = TA0CCR1;
}
void getTime2() {
    time2 = TA0CCR2;
}
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    // Clock setup ---------------------
    // Enter clock control password
    CSCTL0 = CSKEY;

    // Set DCO clock speed to 8 MHz
    CSCTL1 |= DCOFSEL0 + DCOFSEL1; // DCOFSEL = 0b11 => freq = 8MHz
    CSCTL1 &= ~DCORSEL;

    // MCLK = DCO, ACLK = DCO, SMCLK = DCO
    CSCTL2 = SELM0 + SELM1 + SELA0 + SELA1 + SELS0 + SELS1;

    // Set divider to 1 on all clocks
    CSCTL3 = 0;

    // Enter wrong password to disable further clock edits
    CSCTL0_H = 0;

    // End setup -----------------------

    Led_init(0x0F);
    Led_enable(0x03);

    // ===========================================
    // Timer library demo ========================
    // ===========================================

    // [1] Square wave
    // Setup pins
    P2DIR |= BIT1 + BIT2;
    P2SEL0 |= BIT1 + BIT2;
    P2SEL1 &= ~(BIT1 + BIT2);
    // 10Hz square wave, 0.50 duty cycle on pin 1, 0.75 duty cycle on pin 2
    Timer_configureSquareWave(TimerB2, TASSEL__ACLK, 8000000ul, 10, 50, 75);
    // Measure square wave on P2.1.

    // [2] Periodic functions
    Timer_configurePeriodicInterrupt(TimerB1, TASSEL__ACLK, 8000000ul, 3, &flashLed);
    Timer_configurePeriodicInterrupt(TimerB0, TASSEL__ACLK, 8000000ul, 4, &flashLed2);

    // [3] Capture
    // Connect P2.1 to P1.0 or P1.1 with a female-female jumper.

    P1DIR &= ~(BIT0 + BIT1);
    P1SEL0 |= (BIT0 + BIT1);
    P1SEL1 &= ~(BIT0 + BIT1);

    // Initialize timer
    Timer_init(TimerA0, TASSEL__ACLK, ID__8, 1, MC__CONTINUOUS);
    // Connecting to TA0.CC1A andTA0.CC2A.
    // Multiple capture inputs are separately configurable.
    Timer_configureCapture(TimerA0, 1, CM_1, CCIS_0, &getTime);
    Timer_configureCapture(TimerA0, 2, CM_1, CCIS_0, &getTime2);

    // Enable global interrupts
    _EINT();

    while(1);
	return 0;
}
