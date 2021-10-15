/*
 * Useful Headers ============================================
 * Useful Headers ============================================
 * Useful Headers ============================================
 */
#include <msp430.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*
 * Circular Buffer ===========================================
 * Circular Buffer ===========================================
 * Circular Buffer ===========================================
 */
typedef struct {
    unsigned char* dat_;
    int size_;
    volatile unsigned char* volatile ri_; // read pointer - next read from here
    volatile unsigned char* volatile wi_; // write pointer - next write goes here
} Buffer;
#define Buffer_Buffer(varname, size) \
    unsigned char varname_##__LINE__##_buf[size]; \
    Buffer varname = {varname_##__LINE__##_buf, size, varname_##__LINE__##_buf, varname_##__LINE__##_buf};

/*
Usage:
Buffer_Buffer(mybuf, 64); // creates a variable called `mybuf` with buffer size 64
Buffer_write(&mybuf, 10); // mybuf can be used as normal
*/

// Tries to write to buffer; returns false if overflow
bool Buffer_write(Buffer* buf, unsigned char val) {
    volatile unsigned char* wi = buf->wi_;
    *wi++ = val;
    if(wi == buf->dat_ + buf->size_){
        wi = buf->dat_;
    }

    if(wi == buf->ri_) {
        // overflow
        return false;
    }
    buf->wi_ = wi;
    return true;
}

/*
 * Tries to read from buffer. If no data is present, returns -1
 **/
int Buffer_read(Buffer* buf) {
    volatile unsigned char* ri = buf->ri_;
    if (ri == buf->wi_){
        return -1;
    }

    int result = *ri;
    ri++;
    if(ri == buf->dat_ + buf->size_){
        ri = buf->dat_;
    }
    buf->ri_ = ri;
    return result;
}

/*
 * Returns true if the buffer is empty; false otherwise.
 **/
bool Buffer_empty(Buffer* buf) {
    return buf->wi_ == buf->ri_;
}


/*
 * LED Configuration ============================================
 */
inline void Led_init(unsigned char leds);
inline void Led_enable(unsigned char leds);
inline void Led_disable(unsigned char leds);
inline void Led_set(unsigned char leds);
/*
LED Output table:
LED8: P3OUT |= BIT7;
LED7: P3OUT |= BIT6;
LED6: P3OUT |= BIT5;
LED5: P3OUT |= BIT4;
LED4: PJOUT |= BIT3;
LED3: PJOUT |= BIT2;
LED2: PJOUT |= BIT1;
LED1: PJOUT |= BIT0;
*/

/*
 * @brief Initialize LEDs, by configuring P3DIR and PJDIR.
 * @param leds Bitfield containing which LEDs to initialize. LSB is LED1, MSB is LED8
 **/
inline void Led_init(unsigned char leds) {
    P3DIR |= leds & 0xF0;
    PJDIR |= leds & 0x0F;
    Led_disable(0xFF);
}

/*
 * @brief Turn on (enable) LEDs
 * @param leds Bitfield containing which LEDs to turn on.
 **/
inline void Led_enable(unsigned char leds) {
    P3OUT |= leds & 0xF0;
    PJOUT |= leds & 0x0F;
}

/*
 * @brief Turn off (disable) LEDs
 * @param leds Bitfield containing which LEDs to turn off.
 **/
inline void Led_disable(unsigned char leds) {
    P3OUT &= (~leds) & 0xF0;
    PJOUT &= (~leds) & 0x0F;
}

/*
 * @brief Set state of LEDs,
 * @param leds Bitfield containing which LEDs to turn on. Remaining bits will be turned off.
 **/
inline void Led_set(unsigned char leds) {
    Led_disable(~leds);
    Led_enable(leds);
}








/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	return 0;
}
