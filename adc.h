#ifndef MECH423_LIB_ADC_H
#define MECH423_LIB_ADC_H

#include <msp430.h>
#include <stdbool.h>

// ADC variables
volatile unsigned int data = 0; // last read data
volatile bool converted = false; // flag set in ISR

#define NTC_ANALOG_PIN 4

/**
 * @brief Turns on a set number of LEDs.
 * @param value Number of LEDs to turn on. If greater than 8, all LEDs turned on
 *
 * PJDIR and P3DIR must be set to output.
 **/
void setLedLevel(int value) {
    // clear all outputs
    PJOUT &= ~(BIT0 + BIT1 + BIT2 + BIT3);
    P3OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);

    if (value <= 0) {return;}
    // higher cases will fall through to lower cases
    switch(value){
    default:
    case 8: P3OUT |= BIT7;
    case 7: P3OUT |= BIT6;
    case 6: P3OUT |= BIT5;
    case 5: P3OUT |= BIT4;
    case 4: PJOUT |= BIT3;
    case 3: PJOUT |= BIT2;
    case 2: PJOUT |= BIT1;
    case 1: PJOUT |= BIT0;
    case 0: break;
    }
}

/**
 * @brief Measures and returns an analog value on the specified pin
 * @param analogPin which pin to output (e.g. 12 <==> pin A12)
 **/
 unsigned int getAnalogValue(int analogPin) {
    ADC10CTL0 &= ~ADC10ENC; // Disable conversion, so we can change channel
    ADC10MCTL0 = analogPin * ADC10INCH0; // Sample from specified pin
    ADC10CTL0 |= ADC10ENC; // Enable conversion

    converted = false; // Local indicator of conversion
    ADC10CTL0 |= ADC10SC; // Start conversion. ADC10SC is auto-cleared (see p. 437)

    // Wait for conversion to finish
    while (!converted);

    // Output result
    return data;
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
    CSCTL1 &= ~DCORSEL;	// Change the DCO output to the lower output range

    // MCLK = DCO, ACLK = DCO, SMCLK = DCO
    CSCTL2 = SELM0 + SELM1 + SELA0 + SELA1 + SELS0 + SELS1;

    // Set divider to 1 on all clocks
    CSCTL3 = 0;

    // Enter wrong password to disable further clock edits
    CSCTL0_H = 0;

    // LED setup -----------------------
    PJDIR |= BIT0 + BIT1 + BIT2 + BIT3;
    P3DIR |= BIT4 + BIT5 + BIT6 + BIT7;

    // power on P2.7 to power temperature sensor
    P2DIR |= BIT7;
    P2OUT |= BIT7;
    // Configure ADC -------------------------
    ADC10CTL0 &= ~ADC10ENC; // Disable conversion

    ADC10CTL0 = ADC10SHT_6 // Sample and hold for many CLK cycles
            + 0 * ADC10MSC // No multiple conversion
            + ADC10ON // turn on ADC
            ;
    ADC10CTL1 = 0b0 * ADC10SHS0 // Use ADC10SC bit as SHI signal
            + 0b1 * ADC10SHP // SAMPCON sourced from sampling timer, i.e. pulse mode of sampling
            + 0 * ADC10ISSH // Don't invert signal
            + ADC10DIV_0 // No clock division
            + 0b11 * ADC10SSEL0 // Use SMCLK
            + 0b00 * ADC10CONSEQ0 // Single channel, single conversion
            ;
    ADC10CTL2 = 0b1 * ADC10RES; // Set resolution to 10 bits

    ADC10IE = ADC10IE0; // enable interrupts

    __delay_cycles(400); // Delay for Ref to settle

    // Setup UART -----------------------------------

    // Setup P2.0, P2.1 for UART
    P2SEL0 &= ~(BIT0 + BIT1); // for UCA0 TX and RX
    P2SEL1 |= BIT0 + BIT1; // for UCA0 TX and RX

    // Enable UC software reset while modifying settings
    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 &= ~UCPEN; // disable parity
    UCA0CTLW0 &= ~UC7BIT; // set as 8 bit data
    UCA0CTLW0 &= ~UCSPB; // 1 stop bit
    UCA0CTLW0 &= ~(UCMODE0 + UCMODE1); // UART mode
    UCA0CTLW0 |= (UCSSEL1 + UCSSEL0); // Use SMCLK

    // Baud rate setup (9600 baud)
    UCA0BRW = 52; // UCBRx, referencing Table 18-5
    UCA0MCTLW_L = (1 /* = UCBRFx */ << 4) + UCOS16 /* OS16 = 1*/;
    UCA0MCTLW_H = 0x49; // UCBRSx
	UCA0IE |= UCRXIE; // Enable the transmit interrupt

    // Clear UC software reset to enable UART
    UCA0CTLW0 &= ~UCSWRST;

    // Wait for UART transmissions to clear
    while ((UCA0IFG & UCTXIFG) == 0);


    // Enable interrupts and start main loop -------------------
    _EINT();

    // dumb software delay loop
    unsigned long count = 0;
    while(1) {
        int tempReading = getAnalogValue(NTC_ANALOG_PIN) & 0xFF;

        ++count;

        if(count > 2000) {
            // MSB
//        // Wait for Tx buffer to clear
//        while ((UCA0IFG & UCTXIFG) == 0);
//        // Transmit temperature reading
//        UCA0TXBUF = tempReading >> 8;

            // LSB
            // Wait for Tx buffer to clear
            while ((UCA0IFG & UCTXIFG) == 0);
            // Transmit temperature reading
            UCA0TXBUF = tempReading & 0xFF;

            count = 0;
        }

        // Compute # LEDs to use
        // Heating
//        int numLeds = (200 - tempReading) * 7 / 16 + 1;
        // Cooling
        int numLeds = (tempReading - 198)/2;

        if (numLeds < 1) {
            setLedLevel(1);
        } else {
            setLedLevel(numLeds);
        }

//        __delay_cycles(5000000);
    }

	return;
}

#pragma vector=ADC10_VECTOR
__interrupt void conversionDone(void){
    switch(__even_in_range(ADC10IV,12))
    {
    case  0: break;                          // No interrupt
    case  2: break;                          // conversion result overflow
    case  4: break;                          // conversion time overflow
    case  6: break;                          // ADC10HI
    case  8: break;                          // ADC10LO
    case 10: break;                          // ADC10IN
    case 12: data = ADC10MEM0 >> 2;			 // Select the 8 least significant bits
			 converted = true;
             break;
    default: break;
    }
    
}

#endif
