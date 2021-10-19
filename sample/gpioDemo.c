#include <msp430.h>
#include <stdlib.h>

#include "led.h"
#include "gpio.h"


void flashLed1() {
    Led_toggle(0b0001);
}
void flashLed2() {
    Led_toggle(0b0010);
}
void flashLed3() {
    Led_toggle(0b0100);
}
void flashLed4() {
    Led_toggle(0b1000);
}



/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    Led_init(0b00001111);

    // ===========================================
    // GPIO demo =================================
    // ===========================================

    // [1] pinMode, digitalWrite, digitalRead
    // P1.0 output, P1.1 input with pulldown.
    Gpio_pinMode(1, 0, Gpio_PinMode_Output);
    Gpio_pinMode(1, 1, Gpio_PinMode_InputPulldown);
    Gpio_digitalWrite(1, 0, GPIO_HIGH);

    // Connect P1.0 to P1.1 with a female to female jumper. A while loop below
    // does digitalRead() on P1.1 and turns on or off an LED accordingly.

    // [2] Button inputs
    Gpio_configureButton(1, Gpio_Edge_Rising, &flashLed1);
    Gpio_configureButton(2, Gpio_Edge_Falling, &flashLed2);

    // [3] Easier interrupts
    // P1.2 output, P1.3 input.
    Gpio_pinMode(1, 2, Gpio_PinMode_Output);
    Gpio_pinMode(1, 3, Gpio_PinMode_InputPullup);
    Gpio_digitalWrite(1, 2, GPIO_LOW);

    //Connect P1.2 to P1.3 with female to female jumper.
    // Every time you connect, an LED toggles, because this interrupt is run.
    Gpio_onInterruptP1_3 = &flashLed3;
    P1IE |= BIT3; // enable interrupts

    // Enable global interrupts
    _EINT();

    // [1 cont.]
    while(1) {
        if(Gpio_digitalRead(1, 1)) {
            Led_enable(0b1000);
        } else {
            Led_disable(0b1000);
        }
    }
	return;
}
