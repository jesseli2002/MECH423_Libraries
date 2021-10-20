#ifndef MECH423_LIB_UART_H
#define MECH423_LIB_UART_H

#include <stdbool.h>
#include <string.h>

#include "common.h"

/**
 * @brief Flag that indicates whether the UART buffer has overflowed.
 *
 * Gets set by the UART RX ISR, if the circular buffer overflows. You have to reset this flag manually.
*/
bool Uart_overflowed = false;

/*
 *
 * UART Circular Buffer Creation======================================================================
 * UART Circular Buffer Creation======================================================================
 * UART Circular Buffer Creation======================================================================
 *
 */

 /*
  * This code creates a circular buffer for UART read data storage of size BUFFER_SIZE (Default size is 128)
  */
#define BUFFER_SIZE 128

Buffer_Buffer(Uart_buffer_, BUFFER_SIZE); //Name of circular buffer created

 /**
  * @brief This function sets up the UART at 9600 baud. It also allows for UART clock source selection. Finally, it enables UART's interrupts.
  * Usage:
  * Uart_begin(UCSSEL__SMCLK); //Starts UART at 9600 baud with the SMCLK as the source clock
  *
  * @param clockSource One of UCSSEL__UCLK, UCSSEL__ACLK, UCSSEL__SMCLK. Defines the source clock for the UART.
  *
  * Clock source must be running at 8 MHz.
  */

void Uart_begin(unsigned int clockSource)
{
    // Configure ports for UCA0 [ports for UCA0 TX and RX]
    P2SEL0 &= ~(BIT0 + BIT1);
    P2SEL1 |= BIT0 + BIT1;

    UCA0CTLW0 = clockSource;
    UCA0BRW = 52;                        //Referencing Table 18-5. N.b. that this is a decimal input
    UCA0MCTLW = 0x4900 + UCOS16 + UCBRF0;  //Modulation Control Word
    UCA0IE |= UCRXIE;                    //Enable the transmit interrupt
}

float Uart_baudLookupIn_[] = {
    0.0529,
    0.0715,
    0.0835,
    0.1001,
    0.1252,
    0.1430,
    0.1670,
    0.2147,
    0.2224,
    0.2503,
    0.3000,
    0.3335,
    0.3575,
    0.3753,
    0.4003,
    0.4286,
    0.4378,
    0.5002,
    0.5715,
    0.6003,
    0.6254,
    0.6432,
    0.6667,
    0.7001,
    0.7147,
    0.7503,
    0.7861,
    0.8004,
    0.8333,
    0.8464,
    0.8572,
    0.8751,
    0.9004,
    0.9170,
    0.9288,
    100.0
};
unsigned int Uart_baudLookupOut_[] = {
    0x00,
    0x01,
    0x02,
    0x04,
    0x08,
    0x10,
    0x20,
    0x11,
    0x21,
    0x22,
    0x44,
    0x25,
    0x49,
    0x4A,
    0x52,
    0x92,
    0x53,
    0x55,
    0xAA,
    0x6B,
    0xAD,
    0xB5,
    0xB6,
    0xD6,
    0xB7,
    0xBB,
    0xDD,
    0xED,
    0xEE,
    0xBF,
    0xDF,
    0xEF,
    0xF7,
    0xFB,
    0xFD,
    0xFE,
};



/**
 * @brief Sets up the UART at a non-default baud rate for a different clock frequency.
 * @param clockSource One of UCSSEL__UCLK, UCSSEL__ACLK, UCSSEL__SMCLK. Defines the source clock for the UART.
 * @param freq Frequency of the clock source, in Hz
 * @param baud Baud rate. 9600 is most common.
 *
 * Note: For some reason this works for writing to the UART port, but not for reading... fingers crossed it doesn't come up at all
*/
void Uart_beginAtFreq(unsigned int clockSource, unsigned long freq, unsigned long baud){
    // Configure ports for UCA0 [ports for UCA0 TX and RX]
    P2SEL0 &= ~(BIT0 + BIT1);
    P2SEL1 |= BIT0 + BIT1;

    // Enable UC software reset while modifying settings
    UCA0CTLW0 |= UCSWRST;

    // Determine baud rate parameters
    float N_value = ((float)freq) / baud;
    unsigned int os16 = 0;
    unsigned int ucbrx = 0;
    unsigned int ucbrfx = 0;
    unsigned int ucbrsx = 0;

    if (N_value > 16){
        os16 = 1;
        float N_div16 = N_value / 16;

        ucbrx = (int) N_div16;
        ucbrfx = (int)((N_div16 - (int)(N_div16)) * 16);
    } else {
        os16 = 0;
        ucbrx = (int) N_value;
        ucbrfx = 0;
    }

    int i;
    float N_frac = N_value - (int)N_value;
    for (i = 0; i < 36; ++i){
        if(N_frac < Uart_baudLookupIn_[i]) {
            ucbrsx = Uart_baudLookupOut_[i];
            break;
        }
    }

    UCA0BRW = ucbrx; // UCBRx
    UCA0MCTLW_L = (ucbrfx /* = UCBRFx */ << 4) + os16 /* OS16 = 1*/;
    UCA0MCTLW_H = ucbrsx; // UCBRSx
    UCA0CTLW0 = clockSource;

    // Clear UC software reset to enable UART
    UCA0CTLW0 &= ~UCSWRST;
}

 /**
  * This function writes a single character to the UART
  * @param single character to be written to the UART
  * @return void
  */

void Uart_write(char CharacterToWriteToUart)
{
    while ((UCA0IFG & UCTXIFG) == 0); // UCA0IFG: No interrupts pending. UCTXIFG is set when new data can be written into UCAxTXBUF
    UCA0TXBUF = CharacterToWriteToUart;
}

 /**
  * @brief This ISR receives data, and writes this data to a previously created circular buffer called Uart_buffer_
  * ISR vector used: USCI_A0_VECTOR
  * Name of circular buffer used: Uart_buffer_
  * @param void
  * @param return void
  */
#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR_(void)
{
    int RxByte = UCA0RXBUF;                 // Receive byte gets whatever is in the receive buffer
	if (!Buffer_write(&Uart_buffer_, RxByte)){
        Uart_overflowed = true;
    }
}

 /**
  * This ISR receives data, and writes this data to a previously created circular buffer called Uart_buffer_
  * ISR vector used: USCI_A0_VECTOR
  * Name of circular buffer used: Uart_buffer_
  *
  * @brief Reads a single character from the UART circular buffer, or -1 if no data is available.
  * @return -1 if the buffer is empty, else return dequeued value from Uart_buffer_
  */
int Uart_read()
{
    return Buffer_read(&Uart_buffer_);
}
/**
 * @brief returns true if data is available from the circular buffer, false otherwise
 */
bool Uart_hasData()
{
	return !Buffer_empty(&Uart_buffer_);
}

/**
 * @brief Sends a char array to the MSP430 transmission buffer.
 * @param Message Pointer to the message.
 *
 * Example usage:
 * if(QueueIsEmpty)
 * {
 *    Uart_transmitMessage("Error: Queue Empty");
 * }
 **/
void Uart_transmitMessage(const char* Message)
{
    int messageLength = strlen(Message);
    int i;
    for(i = 0; i < messageLength; i++)
    {
        while ((UCA0IFG & UCTXIFG)==0);
        UCA0TXBUF = Message[i];
    }
    return;
}

#endif
