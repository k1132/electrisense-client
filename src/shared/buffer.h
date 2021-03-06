/**
 * @file buffer.h
 * Definition for the shared buffer used by the consumer and relay.
 *
 * The memory shared between the consumer and relay consists of a double buffer
 * with status fields for each buffer. To begin, both buffers are set to be
 * empty. The consumer will then fill begin filling an empty buffer. When the
 * buffer is full, the size field will be equal to the capacity field.
 * This must happen *after* any interaction with the buffer is complete, or else
 * a race condition could occur. The relay will find any full buffer and begin
 * to empty it. When this buffer is empty, the buffer size shall be reset
 * for that buffer. Again, this must happen *after* any interaction with the
 * buffer is complete, or else a race condition could occur.
 */

#ifndef _SHARED_BUFFER_H
#define _SHARED_BUFFER_H

#include <stdlib.h>

/** The capacity of each buffer */
#define __BUFFER_CAPACITY 102400

/**
 * A buffer with status fields.
 *
 * A buffer may be handled by the consumer while its size < its capacity. When
 * the buffer's size == the buffer's capacity, it is full and ready for the
 * relay to process it.
 */
struct buffer_st {
  /** The current size of the buffer */
  size_t size;
  /** The capacity of the buffer */
  size_t capacity;
  /** Data buffer */
  char data[__BUFFER_CAPACITY ];
};

typedef struct buffer_st Buffer;

#endif
