// At the time of writing, the UART modules and clock initialization modules weren't ready yet. 
// Pause to read the temperature value

#include <msp430fr5739.h>
#include "adc.h"

// Global Variables
volatile int temperatureSignal = 0;

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

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
    // End clock setup -----------------


    ADC_init(32, 1, 1, 3);

    // Enable global interrupts
    _EINT();

    while(1)
    {
        temperatureSignal = Adc_read(Adc_NTC_ANALOG_PIN);
    }
    return 0;
}
