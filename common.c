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
    unsigned char varname_##__LINE__##_buf[size]; \
    Buffer varname = {varname_##__LINE__##_buf, size, varname_##__LINE__##_buf, varname_##__LINE__##_buf};

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
