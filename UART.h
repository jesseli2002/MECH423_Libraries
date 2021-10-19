#ifndef MECH423_LIB_UART_H
#define MECH423_LIB_UART_H

#include <common.h>
#include <stdbool.h>
#include <string.h>

/*
 *
 * UART Circular Buffer Creation======================================================================
 * UART Circular Buffer Creation======================================================================
 * UART Circular Buffer Creation======================================================================
 *
 */

 /*
  * This code creates a circular buffer for UART read data storage of size BUFFER_SIZE (Default size is 126)
  */
#define BUFFER_SIZE 0x7E // 126 in hex

Buffer_Buffer(Uart_buffer_, BUFFER_SIZE); //Name of circular buffer created


/*
 *
 * UART INITIALIZATION @ 9600 baud======================================================================
 * UART INITIALIZATION @ 9600 baud======================================================================
 * UART INITIALIZATION @ 9600 baud======================================================================
 *
 */


 /*
  * This function sets up the UART at 9600 baud. It also allows for UART clock source selection. Finally, it enables UART's interrupts.
  * Usage:
  * Uart_begin(3); //Starts UART at 9600 baud with the SMCLK as the source clock
  *
  *@param this function takes an integer value which defines the UART's clock source: 1=UCLK, 2=ACLK, 3=SMCLK, 4=SMCLK
  *@return void
  */

void Uart_begin(int ClockSource)
{
    // Configure ports for UCA0 [ports for UCA0 TX and RX]
    P2SEL0 &= ~(BIT0 + BIT1);
    P2SEL1 |= BIT0 + BIT1;

    // Configure UCA0
    switch (ClockSource) {                    //selects UART's clock source
    case 1: UCA0CTLW0 = UCSSEL__UCLK; break;
    case 2: UCA0CTLW0 = UCSSEL__ACLK; break;
    case 3: UCA0CTLW0 = UCSSEL__SMCLK; break;
    case 4: UCA0CTLW0 = UCSSEL__SMCLK; break;
    }
    UCA0BRW = 52;                        //Referencing Table 18-5. N.b. that this is a decimal input
    UCA0MCTLW = 0x4900 + UCOS16 + UCBRF0;  //Modulation Control Word
    UCA0IE |= UCRXIE;                    //Enable the transmit interrupt
}

/*
 *
 * UART write single character======================================================================
 * UART write single character======================================================================
 * UART write single character======================================================================
 *
 */


 /*
  *This function writes a single character to the UART
  * @parm single character to be written to the UART
  * @parm return void
  */

void Uart_write(char CharacterToWriteToUart)
{
    while ((UCA0IFG & UCTXIFG) == 0); // UCA0IFG: No interrupts pending. UCTXIFG is set when new data can be written into UCAxTXBUF
    UCA0TXBUF = CharacterToWriteToUart;
}


/*
 *
 * UART interrupt service routine (ISR) implementation======================================================================
 * UART interrupt service routine (ISR) implementation======================================================================
 * UART interrupt service routine (ISR) implementation======================================================================
 *
 */


 /*
  * You need to create a buffer before using this function using "void uartBufferCreation(int sizeOfBuffer)" from above.
  *This ISR receives data, and writes this data to a previously created circular buffer called Uart_buffer_
  *ISR vector used: USCI_A0_VECTOR
  *Name of circular buffer used: Uart_buffer_
  * @param void
  * @param return void
  */


#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    RxByte = UCA0RXBUF;                 // Receive byte gets whatever is in the receive buffer
    while ((UCA0IFG & UCTXIFG) == 0);     // UCA0IFG: No interrupts pending. UCTXIFG is set when new data can be written into UCAxTXBUF
	Buffer_write(Uart_buffer_, RxByte);
}

/*
 *
 * UART read character======================================================================
 * UART read character======================================================================
 * UART read character======================================================================
 *
 */


 /*
  * This ISR receives data, and writes this data to a previously created circular buffer called Uart_buffer_
  * ISR vector used: USCI_A0_VECTOR
  * Name of circular buffer used: Uart_buffer_
  *
  * @brief Reads a single character from the UART circular buffer, or -1 if no data is available.
  * @param void
  * @param return -1 if the buffer is empty, else return dequeued value from Uart_buffer_
  */
int Uart_read()
{
	if((Uart_buffer_) == true)
	{
		return -1;
	}
	else
	{
		return Buffer_read(Uart_buffer_);
	}
}
/*
 * @brief returns true if data is available from the circular buffer, false otherwise
 * @param
 */
bool Uart_hasData()
{
	if(Buffer_empty(Uart_buffer_) == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief Sends a char array to the MSP430 transmission buffer.
 * @param Message Pointer to the message.
 * @return void
 *
 * Example usage:
 * if(QueueIsEmpty)
 * {
 * 	  char ErrorMessageEmpty[] = "Error: Queue Empty";
 *    Uart_transmitMessage(ErrorMessageEmpty);
 * }

 **/

void Uart_transmitMessage(char* Message)
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
