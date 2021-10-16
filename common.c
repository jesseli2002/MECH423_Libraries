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

/*
Usage:
Buffer_Buffer(mybuf, 64); // creates a variable called `mybuf` with buffer size 64
Buffer_write(&mybuf, 10); // mybuf can be used as a normal variable
*/

// Struct definition
typedef struct {
    unsigned char* dat_;
    int size_;
    volatile unsigned char* volatile ri_; // read pointer - next read from here
    volatile unsigned char* volatile wi_; // write pointer - next write goes here
} Buffer;
#define Buffer_Buffer(varname, size) \
    unsigned char varname ## _UNDERLYING_BUFFER[size]; \
    Buffer varname = {varname ## _UNDERLYING_BUFFER, size, varname ## _UNDERLYING_BUFFER, varname ## _UNDERLYING_BUFFER};

/**
 * @brief Writes (enqueues) data into the buffer
 * @param buf Pointer to buffer
 * @param val Value to add to buffer
 * @return True if successful, false if there was overflow (too much data in buffer). In case of overflow, no data is written.
*/
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

/**
 * @brief Reads (dequeues) data from buffer
 * @param buf Pointer to buffer
 * @return Read value if successful, or -1 if no data available.
*/
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

/**
 * @brief Checks if the buffer is empty.
 * @param buf Pointer to buffer
 * @return True if the buffer is empty; false otherwise.
 **/
bool Buffer_empty(Buffer* buf) {
    return buf->wi_ == buf->ri_;
}

/*
 * Packet Parsing ===========================================
 * Packet Parsing ===========================================
 * Packet Parsing ===========================================
 */

// Start byte of the packet. Needs to be escaped with the escape byte
#define PACKET_START_BYTE 0xFF

// Parser state. Used to track where in the packet we are
typedef enum {
    Packet_ParseState_err, // error state. For convenience, this is also the initial state.

    // Remainder of states indicate which byte is about to be read
    Packet_ParseState_cmd,
    Packet_ParseState_dat0,
    Packet_ParseState_dat1,
    Packet_ParseState_esc,
    Packet_ParseState_done
}  Packet_ParseState;

typedef struct {
    // raw packet bytes
    unsigned char cmd;
    unsigned char dat0;
    unsigned char dat1;
    unsigned char esc;

    // combined data
    unsigned int dat;

    // state
    Packet_ParseState state;
} Packet;

// Define commands - this is just a sample
enum Command {
    Command_setFreq = 1,
    Command_ledOn = 2,
    Command_ledOff = 3,
    Command_echo = 4,
    Command_setDutyCycle = 5,
    Command_ledCtl = 6,
    Command_echoString = 7
};

/**
 * @brief Incremental parser for a packet of data.
 * @param packet Pointer to packet. Packet gets filled out as more data is parsed
 * @param next_val Next read value.
 * @return True if reached the end of a packet (i.e. full packet is available), false otherwise.
 *
 * When a packet is finished reading, dat0 and dat1 will be set to their correct values, taking into consideration escaping.
 *
 * Example usage:
 * Packet packet;
 * while (true) {
 *     int data = Uart_read();
 *     if(Packet_parseNext(&packet, data)) {
 *          // Full packet available; do something with the packet
 *     }
 * }
 **/
bool Packet_parseNext(Packet* packet, unsigned char next_val){
    if (next_val == PACKET_START_BYTE) {
        // Throw everything out and start from the top.
        packet->state = Packet_ParseState_cmd;
        return false;
    }

    // Otherwise, we have normal data
    switch(packet->state){
    case Packet_ParseState_cmd:
        packet->cmd = next_val;
        packet->state = Packet_ParseState_dat0;
        return false;
    case Packet_ParseState_dat0:
        packet->dat0 = next_val;
        packet->state = Packet_ParseState_dat1;
        return false;
    case Packet_ParseState_dat1:
        packet->dat1 = next_val;
        packet->state = Packet_ParseState_esc;
        return false;
    case Packet_ParseState_esc:
        packet->esc = next_val;

        // Deal with escaping. BIT0 enables dat0, and BIT1 enables DAT1.
        if (next_val & BIT0) {
            packet->dat0 = PACKET_START_BYTE;
        }
        if (next_val & BIT1) {
            packet->dat1 = PACKET_START_BYTE;
        }

        // Merge dat0 and dat1 into a single 16 bit integer
        packet->dat = (((unsigned int)packet->dat0) << 8) + packet->dat1;

        //  Mark everything as done
        packet->state = Packet_ParseState_done;
        return true;

    case Packet_ParseState_err:
        // error state - we have no idea where we are, so all we can do is wait for the next start byte
    case Packet_ParseState_done:
        // If we're done, then next byte should've been START_BYTE, and the if statement at the start of this function should've triggered. Sincre we're here, evidently it didn't, so there's been a problem
    default:
        packet->state = Packet_ParseState_err;
        return false;
    }
}
