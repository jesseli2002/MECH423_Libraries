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

 /**
  * This function writes a single character to the UART
  * @param single character to be written to the UART
  * @param return void
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
