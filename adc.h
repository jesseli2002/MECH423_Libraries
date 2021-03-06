#ifndef MECH423_LIB_ADC_H
#define MECH423_LIB_ADC_H

#include <msp430.h>
#include <stdbool.h> // required to use Booleans in C

// ADC variables
volatile unsigned int Adc_data_ = 0; // last read data
volatile bool Adc_converted_ = false; // flag set in ISR

#define Adc_NTC_ANALOG_PIN 4
#define Adc_ACCELEROMETER_X 12
#define Adc_ACCELEROMETER_Y 13
#define Adc_ACCELEROMETER_Z 14

/**
 * @brief Measures and returns an analog value on the specified pin
 * @param analogPin which pin to output (e.g. 12 <==> pin A12)
 * 		  NTC Sensor: Pin 4
 * 		  AccelerometerX: Pin 12
 * 		  AccelerometerY: Pin 13
 * 		  AccelerometerZ: Pin 14
 * @return Returns the 10 bit result inside of a 16 bit int. Bit shift with result >> 2 to return 8 MSB. cast to char or write data & 0xFF to return 8 LSB (better accuracy)
 **/
 unsigned int Adc_read(int analogPin) {
    ADC10CTL0 &= ~ADC10ENC; // Disable conversion, so we can change channel
    ADC10MCTL0 = analogPin * ADC10INCH0; // Sample from specified pin
    ADC10CTL0 |= ADC10ENC; // Enable conversion

    Adc_converted_ = false; // Local indicator of conversion
    ADC10CTL0 |= ADC10SC; // Start conversion. ADC10SC is auto-cleared (see p. 437)

    // Wait for conversion to finish
    while (!Adc_converted_);

    // Output result
    return Adc_data_;
}


/**
 * @brief
 * @param sampleHoldCycles 4 / 8 / 16 / 32 / 64 / 96 / 128 / 192 / 256 / 384 / 512 / 768 / 1024
 *                         these values are the only valid param values.
 * @param preDivider       1 / 4 / 64
 * @param divider          1 / 2 / 3 / 4 / 5 / 6 / 7 / 8
 * @param clockSource 	   0 : MODCLK
 *                         1 : ACLK
 *                         2 : MCLK
 *                         3 : SMCLK
 *
 * Exact counts probably don't matter, so sampleHoldCycles = 16, preDivider = divider = 1 is probably fine.
 * */
void ADC_init(int sampleHoldCycles, int preDivider, int divider, unsigned int clockSource)
{
	ADC10CTL0 &= ~ADC10ENC;                 // Disable conversion
	ADC10CTL0 |= ADC10ON;                   // Activate the ADC

    switch(sampleHoldCycles)
	{
		case 4:
			ADC10CTL0 |= ADC10SHT_0;
			break;
		case 8:
			ADC10CTL0 |= ADC10SHT_1;
			break;
		case 16:
			ADC10CTL0 |= ADC10SHT_2;
			break;
		case 32:
			ADC10CTL0 |= ADC10SHT_3;
			break;
		case 64:
			ADC10CTL0 |= ADC10SHT_4;
			break;
		case 96:
			ADC10CTL0 |= ADC10SHT_5;
			break;
		case 128:
			ADC10CTL0 |= ADC10SHT_6;
			break;
		case 192:
			ADC10CTL0 |= ADC10SHT_7;
			break;
		case 256:
			ADC10CTL0 |= ADC10SHT_8;
			break;
		case 384:
			ADC10CTL0 |= ADC10SHT_9;
			break;
		case 512:
			ADC10CTL0 |= ADC10SHT_10;
			break;
		case 768:
			ADC10CTL0 |= ADC10SHT_11;
			break;
		case 1024:
			ADC10CTL0 |= ADC10SHT_12;
			break;
		default:
			break;
	}

    ADC10CTL1 |= ADC10SHS_0;                // Set the source as ADC10SC bit
    ADC10CTL1 |= ADC10SHP;                  // Set the source of Sampling Signal (SAMPCON) as the sampling timer
    ADC10CTL1 |= ADC10CONSEQ_0;             // Single channel, single conversion

	switch(divider)
	{
		case 1:
			ADC10CTL1 |= ADC10DIV_0;                // ADC10 Clock Divider Select /1
			break;
		case 2:
			ADC10CTL1 |= ADC10DIV_1;                // ADC10 Clock Divider Select /2
			break;
		case 3:
			ADC10CTL1 |= ADC10DIV_2;                // ADC10 Clock Divider Select /3
			break;
		case 4:
			ADC10CTL1 |= ADC10DIV_3;                // ADC10 Clock Divider Select /3
			break;
		case 5:
			ADC10CTL1 |= ADC10DIV_4;                // ADC10 Clock Divider Select /3
			break;
		case 6:
			ADC10CTL1 |= ADC10DIV_5;                // ADC10 Clock Divider Select /3
			break;
		case 7:
			ADC10CTL1 |= ADC10DIV_6;                // ADC10 Clock Divider Select /3
			break;
		case 8:
			ADC10CTL1 |= ADC10DIV_7;                // ADC10 Clock Divider Select /3
			break;
		default:
			break;
	}

	switch(preDivider)
	{
		case 1:
			ADC10CTL1 |= ADC10PDIV_0;                // ADC10 Pre Clock Divider Select /3
			break;
		case 4:
			ADC10CTL1 |= ADC10PDIV_1;                // ADC10 Pre Clock Divider Select /4
			break;
		case 64:
			ADC10CTL1 |= ADC10PDIV_2;                // ADC10 Pre Clock Divider Select /64
			break;
		default:
			break;
	}

	switch(clockSource)
	{
		case 0:
			ADC10CTL1 |= ADC10SSEL_0;                // ADC10 Pre Clock Divider Select /3
			break;
		case 1:
			ADC10CTL1 |= ADC10SSEL_1;                // ADC10 Pre Clock Divider Select /4
			break;
		case 2:
			ADC10CTL1 |= ADC10SSEL_2;                // ADC10 Pre Clock Divider Select /64
			break;
		case 3:
			ADC10CTL1 |= ADC10SSEL_3;                // ADC10 Pre Clock Divider Select /64
			break;
	}


    ADC10CTL2 |= ADC10RES;                  // 10-bit conversion results
	ADC10IE = ADC10IE0;                     // enable conversion complete interrupts

	// Power Up Sensors
	P2DIR |= BIT7;                           // Setting P2.7 as an output
    P2OUT |= BIT7;                           // Setting P2.7 to output HIGH

	return;
}


#pragma vector=ADC10_VECTOR
__interrupt void Adc_conversionDone_(void){
    switch(__even_in_range(ADC10IV,12))
    {
    case  0: break;                          // No interrupt
    case  2: break;                          // conversion result overflow
    case  4: break;                          // conversion time overflow
    case  6: break;                          // ADC10HI
    case  8: break;                          // ADC10LO
    case 10: break;                          // ADC10IN
    case 12: Adc_data_ = ADC10MEM0;			     //
			 Adc_converted_ = true;
             break;
    default: break;
    }

}

#endif
