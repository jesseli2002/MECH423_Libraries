#include <stdlib.h>
/*
Capabilities:

[1] pinMode, digitalRead, and digitalWrite capabilities

[2] Automatic configuration of button inputs

[3] Easier ISRs - each of the function pointers below can be modified, and correspond to an interrupt on a port and pin.
By setting them to your own function, you don't need to use the interrupt pragmas.
You still need to setup interrupt enable & pin modes though, as well as call _EINT();
*/
void (*Gpio_onInterruptP1_0)(void) = NULL;
void (*Gpio_onInterruptP1_1)(void) = NULL;
void (*Gpio_onInterruptP1_2)(void) = NULL;
void (*Gpio_onInterruptP1_3)(void) = NULL;
void (*Gpio_onInterruptP1_4)(void) = NULL;
void (*Gpio_onInterruptP1_5)(void) = NULL;
void (*Gpio_onInterruptP1_6)(void) = NULL;
void (*Gpio_onInterruptP1_7)(void) = NULL;
void (*Gpio_onInterruptP2_0)(void) = NULL;
void (*Gpio_onInterruptP2_1)(void) = NULL;
void (*Gpio_onInterruptP2_2)(void) = NULL;
void (*Gpio_onInterruptP2_3)(void) = NULL;
void (*Gpio_onInterruptP2_4)(void) = NULL;
void (*Gpio_onInterruptP2_5)(void) = NULL;
void (*Gpio_onInterruptP2_6)(void) = NULL;
void (*Gpio_onInterruptP2_7)(void) = NULL;
void (*Gpio_onInterruptP3_0)(void) = NULL;
void (*Gpio_onInterruptP3_1)(void) = NULL;
void (*Gpio_onInterruptP3_2)(void) = NULL;
void (*Gpio_onInterruptP3_3)(void) = NULL;
void (*Gpio_onInterruptP3_4)(void) = NULL;
void (*Gpio_onInterruptP3_5)(void) = NULL;
void (*Gpio_onInterruptP3_6)(void) = NULL;
void (*Gpio_onInterruptP3_7)(void) = NULL;
void (*Gpio_onInterruptP4_0)(void) = NULL;
void (*Gpio_onInterruptP4_1)(void) = NULL;
void (*Gpio_onInterruptP4_2)(void) = NULL;
void (*Gpio_onInterruptP4_3)(void) = NULL;
void (*Gpio_onInterruptP4_4)(void) = NULL;
void (*Gpio_onInterruptP4_5)(void) = NULL;
void (*Gpio_onInterruptP4_6)(void) = NULL;
void (*Gpio_onInterruptP4_7)(void) = NULL;


// Define all the ISRs
#define GPIO_CALL_IF_EXISTS(fcn) if(fcn != NULL) {fcn();}

// Port interrupts
#pragma vector=PORT1_VECTOR
__interrupt void Gpio_port1Isr_(){
    switch (__even_in_range(P1IV, 0x10)) {
    case 0x0: break; // nothing
    case 0x2: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP1_0); break;
    case 0x4: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP1_1); break;
    case 0x6: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP1_2); break;
    case 0x8: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP1_3); break;
    case 0xA: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP1_4); break;
    case 0xC: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP1_5); break;
    case 0xE: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP1_6); break;
    case 0x10: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP1_7); break;
    }
}
#pragma vector=PORT2_VECTOR
__interrupt void Gpio_port2Isr_(){
    switch (__even_in_range(P2IV, 0x10)) {
    case 0x0: break; // nothing
    case 0x2: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP2_0); break;
    case 0x4: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP2_1); break;
    case 0x6: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP2_2); break;
    case 0x8: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP2_3); break;
    case 0xA: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP2_4); break;
    case 0xC: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP2_5); break;
    case 0xE: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP2_6); break;
    case 0x10: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP2_7); break;
    }
}
#pragma vector=PORT3_VECTOR
__interrupt void Gpio_port3Isr_(){
    switch (__even_in_range(P3IV, 0x10)) {
    case 0x0: break; // nothing
    case 0x2: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP3_0); break;
    case 0x4: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP3_1); break;
    case 0x6: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP3_2); break;
    case 0x8: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP3_3); break;
    case 0xA: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP3_4); break;
    case 0xC: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP3_5); break;
    case 0xE: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP3_6); break;
    case 0x10: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP3_7); break;
    }
}

#pragma vector=PORT4_VECTOR
__interrupt void Gpio_port4Isr_(){
    switch (__even_in_range(P4IV, 0x10)) {
    case 0x0: break; // nothing
    case 0x2: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP4_0); break;
    case 0x4: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP4_1); break;
    case 0x6: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP4_2); break;
    case 0x8: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP4_3); break;
    case 0xA: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP4_4); break;
    case 0xC: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP4_5); break;
    case 0xE: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP4_6); break;
    case 0x10: GPIO_CALL_IF_EXISTS(Gpio_onInterruptP4_7); break;
    }
}

typedef enum {
    Gpio_PinMode_Input, // no resistor
    Gpio_PinMode_InputPullup, // pullup resistor
    Gpio_PinMode_InputPulldown, // pulldown resistor
    Gpio_PinMode_Output
} Gpio_PinMode;

/**
 * @brief Configures a pin for digital input or output. The corresponding PxSEL1 and PxSEL0 are set to 0 as a result.
 * @param port Which port to use - one of 1, 2, 3, 4
 * @param pin Which pin to use - number from 0 to 7
 * @param mode Whether pin should be configured as output, input without resistor,
 *             input with pullup resistor, or input with pulldown resistor; see Gpio_PinMode enum
 **/
void Gpio_pinMode(int port, int pin, Gpio_PinMode mode) {
    unsigned bit = 1 << pin;
    switch(mode) {
    case Gpio_PinMode_Input:
        switch(port){
        case 1: P1DIR &= ~bit; P1REN &= ~bit; break;
        case 2: P2DIR &= ~bit; P2REN &= ~bit; break;
        case 3: P3DIR &= ~bit; P3REN &= ~bit; break;
        case 4: P4DIR &= ~bit; P4REN &= ~bit; break;
        }
        break;
    case Gpio_PinMode_InputPullup:
        switch(port){
        case 1: P1DIR &= ~bit; P1REN |= bit; P1OUT |= bit; break;
        case 2: P2DIR &= ~bit; P2REN |= bit; P2OUT |= bit; break;
        case 3: P3DIR &= ~bit; P3REN |= bit; P3OUT |= bit; break;
        case 4: P4DIR &= ~bit; P4REN |= bit; P4OUT |= bit; break;
        }
        break;
    case Gpio_PinMode_InputPulldown:
        switch(port){
        case 1: P1DIR &= ~bit; P1REN |= bit; P1OUT &= ~bit; break;
        case 2: P2DIR &= ~bit; P2REN |= bit; P2OUT &= ~bit; break;
        case 3: P3DIR &= ~bit; P3REN |= bit; P3OUT &= ~bit; break;
        case 4: P4DIR &= ~bit; P4REN |= bit; P4OUT &= ~bit; break;
        }
        break;
    case Gpio_PinMode_Output:
        switch(port){
        case 1: P1DIR |= bit; break;
        case 2: P2DIR |= bit; break;
        case 3: P3DIR |= bit; break;
        case 4: P4DIR |= bit; break;
        }
        break;
    }

    switch(port){
    case 1: P1SEL0 &= ~bit; P1SEL0 &= ~bit; break;
    case 2: P2SEL0 &= ~bit; P2SEL0 &= ~bit; break;
    case 3: P3SEL0 &= ~bit; P3SEL0 &= ~bit; break;
    case 4: P4SEL0 &= ~bit; P4SEL0 &= ~bit; break;
    }
}

#define GPIO_HIGH 1
#define GPIO_LOW 0
/**
 * @brief Drives a pin high or low. Must call Gpio_pinMode() first.
 * @param port Which port to use - one of 1, 2, 3, 4
 * @param pin Which pin to use - number from 0 to 7
 * @param mode Either GPIO_HIGH or GPIO_LOW - see the defined macros above
 */
void Gpio_digitalWrite(int port, int pin, int mode) {
    unsigned int bit = 1 << pin;
    if (mode == GPIO_HIGH) {
        switch(port){
        case 1: P1OUT |= bit; break;
        case 2: P2OUT |= bit; break;
        case 3: P3OUT |= bit; break;
        case 4: P4OUT |= bit; break;
        }
    } else {
        switch(port){
        case 1: P1OUT &= ~bit; break;
        case 2: P2OUT &= ~bit; break;
        case 3: P3OUT &= ~bit; break;
        case 4: P4OUT &= ~bit; break;
        }
    }
}

/**
 * @brief Reads the value at a pin. Must call Gpio_pinMode() first.
 * @param port Which port to use - one of 1, 2, 3, 4
 * @param pin Which pin to use - number from 0 to 7
 * @return Either GPIO_HIGH or GPIO_LOW - see the defined macros above
 */
int Gpio_digitalRead(int port, int pin) {
    unsigned int bit = 1 << pin;
    switch(port){
    case 1: return P1IN & bit;
    case 2: return P2IN & bit;
    case 3: return P3IN & bit;
    case 4: return P4IN & bit;
    }
    return 0;
}

typedef enum {
    Gpio_Edge_Rising = 0,
    Gpio_Edge_Falling = 1
} Gpio_Edge;

/**
 * @brief Configures one of the buttons to call a function when triggered.
 * @param button Either 1 or 2, corresponding to button 1 or 2.
 * @param edgeType Whether to detect rising or falling edges; see Gpio_Edge enum.
 * @param function Function pointer to call on
 *
 * This function includes calling Gpio_pinMode to configure the pin with pullups.
 * To disable the interrupt, pass NULL to `function`.
 * This function does not call _EINT(); you need to do that to enable global interrupts.
 * Falling edge occurs when you push down the button; rising edge occurs on release.
 * */
void Gpio_configureButton(int button, Gpio_Edge edgeType, void (*function)(void)) {
    if (button == 1) {
        Gpio_pinMode(4, 0, Gpio_PinMode_InputPullup);
        P4IE |= BIT0;
        P4IES = (P4IES & ~BIT0) + edgeType;
        Gpio_onInterruptP4_0 = function;
    } else {
        Gpio_pinMode(4, 1, Gpio_PinMode_InputPullup);
        P4IE |= BIT1;
        P4IES = (P4IES & ~BIT0) + edgeType << 1;
        Gpio_onInterruptP4_1 = function;
    }
}
