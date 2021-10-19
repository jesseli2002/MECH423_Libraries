#ifndef MECH423_LIB_LED_H
#define MECH423_LIB_LED_H
/*
 * LED Configuration ============================================
 * LED Configuration ============================================
 * LED Configuration ============================================
 */
inline void Led_init(unsigned char leds);
inline void Led_enable(unsigned char leds);
inline void Led_disable(unsigned char leds);
inline void Led_set(unsigned char leds);
inline void Led_toggle(unsigned char leds);

/*
LED Output table:
LED8: P3, BIT7
LED7: P3, BIT6
LED6: P3, BIT5
LED5: P3, BIT4
LED4: PJ, BIT3
LED3: PJ, BIT2
LED2: PJ, BIT1
LED1: PJ, BIT0
*/

/**
 * @brief Initialize LEDs, by configuring P3DIR and PJDIR. Turns off all LEDs as part of initialization
 * @param leds Bitfield containing which LEDs to initialize. LSB is LED1, MSB is LED8
 **/
inline void Led_init(unsigned char leds) {
    P3DIR |= leds & 0xF0;
    PJDIR |= leds & 0x0F;
    Led_disable(0xFF);
}

/**
 * @brief Turn on (enable) LEDs
 * @param leds Bitfield containing which LEDs to turn on.
 *
 * E.g. Led_enable(BIT0 + BIT4) turns on LED0 and LED4.
 **/
inline void Led_enable(unsigned char leds) {
    P3OUT |= leds & 0xF0;
    PJOUT |= leds & 0x0F;
}

/**
 * @brief Turn off (disable) LEDs
 * @param leds Bitfield containing which LEDs to turn off.
 *
 * E.g. Led_disable(BIT0 + BIT4) turns off LED0 and LED4.
 **/
inline void Led_disable(unsigned char leds) {
    P3OUT &= (~leds) & 0xF0;
    PJOUT &= (~leds) & 0x0F;
}

/**
 * @brief Set state of LEDs,
 * @param leds Bitfield containing which LEDs to turn on. Remaining bits will be turned off.
 *
 * E.g. Led_set(BIT0 + BIT4) turns on LED0 and LED4, and turns off all other LEDs.
 **/
inline void Led_set(unsigned char leds) {
    Led_disable(~leds);
    Led_enable(leds);
}

/**
 * @brief Toggles some LEDs
 * @param leds Bitfield containing which LEDs to toggle
 **/
inline void Led_toggle(unsigned char leds){
    P3OUT ^= leds & 0xF0;
    PJOUT ^= leds & 0x0F;
}

/**
 * @brief Turns on a set number of LEDs, in order from 0 to 8
 * @param value Number of LEDs to turn on. If greater than 8, all LEDs turned on. If less than 0, no LEDs turned on.
 *
 * PJDIR and P3DIR must be set to output, e.g. by calling Led_init(0xFF);
 **/
void Led_setLevel(int value) {
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
#endif
