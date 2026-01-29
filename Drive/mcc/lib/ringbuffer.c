// C
#include "ringbuffer.h"

/*
    Critical section handling:
    The ring buffer may be accessed from multiple contexts (e.g., ISR and main).
    To avoid race conditions, we protect updates with critical sections using user-provided hooks.
    The hooks are called before and after each operation and can implement synchronization,
    for example by disabling interrupts.

    If you write to the ring buffer from the UART RX interrupt, consider disabling the RX interrupt
    inside the corresponding critical section hook used by the non-ISR side to prevent concurrent access.

    The functions RingBuffer_SetWriteLock/Unlock and RingBuffer_SetReadLock/Unlock configure these hooks.
*/

static inline uint16_t rb_next(uint16_t idx, uint16_t size) {
    idx++;
    if (idx >= size) idx = 0;
    return idx;
}

// Helper: check if a byte is a line terminator
static inline bool rb_is_line_term(uint8_t ch) {
    return (ch == '\n') || (ch == '\r') || (ch == '\0');
} 

// Initializes the ring buffer
#if DYNAMIC_BUFFERSIZE == 1
void RingBuffer_Init(volatile RingBuffer *rb, uint16_t buffer_size)
{
    rb->buffer = (char *)malloc(buffer_size); // Allocate storage for the buffer
    if (rb->buffer == NULL) {
        // Error handling: allocation failed
        rb->size = 0;
        return;
    }
    rb->size = buffer_size; // Store buffer size
#else
void RingBuffer_Init(volatile RingBuffer *rb)
{
    // Use a static buffer size
    rb->size = STATIC_BUFFER_SIZE;
    // Optionally initialize the static buffer (e.g., with zeros)
    memset((void *)rb->buffer, 0, rb->size);
#endif
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    rb->write_lock = NULL;
    rb->write_unlock = NULL;
    rb->read_lock = NULL;
    rb->read_unlock = NULL;
}

void RingBuffer_SetWriteLock(volatile RingBuffer *rb, rb_hook_t hook) {
    rb->write_lock = hook;
}

void RingBuffer_SetWriteUnlock(volatile RingBuffer *rb, rb_hook_t hook) {
    rb->write_unlock = hook;
}

void RingBuffer_SetReadLock(volatile RingBuffer *rb, rb_hook_t hook) {
    rb->read_lock = hook;
}

void RingBuffer_SetReadUnlock(volatile RingBuffer *rb, rb_hook_t hook) {
    rb->read_unlock = hook;
}

// Releases buffer memory (only required for dynamic allocation)
void RingBuffer_Free(volatile RingBuffer *rb) {
#if DYNAMIC_BUFFERSIZE == 1
    if (rb->buffer != NULL) {
        free(rb->buffer); // Free the allocated memory
        rb->buffer = NULL;
    }
#endif
    // No action needed for static buffers
    rb->size = 0;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    rb->line_count = 0;
}

// Returns true if the ring buffer is full
bool RingBuffer_IsFull(volatile RingBuffer *rb) {
    if (rb->write_lock) rb->write_lock();
    if (rb->read_lock) rb->read_lock();
    bool is_full = rb->count == rb->size; // Compare against actual capacity
    if (rb->write_unlock) rb->write_unlock();
    if (rb->read_unlock) rb->read_unlock();
    return is_full;
}

// Returns true if the ring buffer is empty
bool RingBuffer_IsEmpty(volatile RingBuffer *rb) {
    if (rb->write_lock) rb->write_lock();
    if (rb->read_lock) rb->read_lock();
    bool is_empty = rb->count == 0;
    if (rb->write_unlock) rb->write_unlock();
    if (rb->read_unlock) rb->read_unlock();
    return is_empty;
}

// Writes a single byte into the ring buffer
bool RingBuffer_Write(volatile RingBuffer *rb, char data) {
    if (RingBuffer_IsFull(rb)) return false;  // Buffer is full
    if (rb->read_lock) rb->read_lock();
    rb->buffer[rb->head] = data;              // Store at head position
    rb->head = (rb->head + 1) % rb->size;     // Advance head (wrap-around)
    rb->count++;                              // Increment element count

    if (rb_is_line_term(data)) {
        rb->line_count++; // one more complete line available
    }
    if (rb->read_unlock) rb->read_unlock();
    return true;
}

// Reads a single byte from the ring buffer
bool RingBuffer_Read(volatile RingBuffer *rb, char *data) {
    if (RingBuffer_IsEmpty(rb)) return false; // Buffer is empty
    if (rb->write_lock) rb->write_lock();
    *data = rb->buffer[rb->tail];             // Load from tail position
    rb->tail = (rb->tail + 1) % rb->size;     // Advance tail (wrap-around)
    rb->count--;                              // Decrement element count
    if (rb->write_unlock) rb->write_unlock();
    return true;
}
// C
bool RingBuffer_WriteString(volatile RingBuffer *rb, const char *str) {
    const char *ptr = str;
    uint16_t length = (uint16_t)strlen(str);
    bool ok = true;

    if (rb->read_lock) rb->read_lock();

    if (rb->size - rb->count < length) {
        ok = false;
    } else {
        rb->count += length;

        uint16_t head = rb->head, size = rb->size;
        uint16_t first = size - head;
        if (first > length) first = length;

        // copy first chunk
        memcpy((char*)&rb->buffer[head], ptr, first);
        // count terminators in first chunk
        for (uint16_t i = 0; i < first; ++i) {
            if (rb_is_line_term(ptr[i])) rb->line_count++; // NEW
        }

        uint16_t remaining = length - first;
        if (remaining) {
            memcpy((char*)&rb->buffer[0], ptr + first, remaining);
            // count terminators in remaining chunk
            for (uint16_t i = 0; i < remaining; ++i) {
                if (rb_is_line_term(ptr[first + i])) rb->line_count++; // NEW
            }
        }

        rb->head = (head + length) % size;
    }

    if (rb->read_unlock) rb->read_unlock();
    return ok;
}
/*
// Writes a null-terminated string into the ring buffer
bool RingBuffer_WriteString(volatile RingBuffer *rb, const char *str) {
    const char *ptr = str;
    uint16_t length = strlen(str);
    bool ok = true;

    // Check if there is enough free space for the string (excluding '\0')
    if (rb->read_lock) rb->read_lock();
    if (rb->size - rb->count < length) ok = false;
    else {
        rb->count += length;
        // Copy the string into the ring buffer with wrap-around handling
        uint16_t head = rb->head, size = rb->size;
        uint16_t first = size - head;
        if (first > length) first = length;
        memcpy((char*)&rb->buffer[head], ptr, first);
        uint16_t remaining = length - first;
        if (remaining) memcpy((char*)&rb->buffer[0], ptr + first, remaining);
        rb->head = (head + length) % size; // or & (size-1) if size is a power of two
    }
    if (rb->read_unlock) rb->read_unlock();
    return ok;
}
*/

  

uint16_t RingBuffer_NumberOfLines(volatile RingBuffer* rb)
{
    // Light-weight: just read; optionally guard with read_lock for atomicity
    if (rb->read_lock) rb->read_lock();
    uint16_t lines = rb->line_count;
    if (rb->read_unlock) rb->read_unlock();
    return lines;
}


bool RingBuffer_ReadLine(volatile RingBuffer* rb, char* out, uint16_t outSize)
{
    if (out == NULL || outSize == 0) return false;

    // Wenn keine komplette Zeile im Puffer ist, sofort abbrechen
    if (rb->line_count == 0) {
        out[0] = '\0';
        return false;
    }

    if (rb->write_lock) rb->write_lock();

    // Doppelt prüfen nach Lock, falls paralleler Schreiber gerade gezählt hat
    if (rb->line_count == 0 || rb->count == 0) {
        if (rb->write_unlock) rb->write_unlock();
        out[0] = '\0';
        return false;
    }

    // Zeichen bis zum ersten Terminator kopieren (mit Truncation)
    uint16_t copied = 0;
    uint16_t toCopyMax = (outSize > 0) ? (uint16_t)(outSize - 1) : 0;

    while (rb->count > 0) {
        char ch = rb->buffer[rb->tail];
        if (rb_is_line_term(ch)) break;

        // kopieren oder verwerfen bei Überlänge
        if (copied < toCopyMax) {
            out[copied++] = ch;
        }
        // in jedem Fall konsumieren
        rb->tail = rb_next(rb->tail, rb->size);
        rb->count--;
    }

    // Jetzt muss am Kopf ein Terminator stehen (wir wissen: line_count > 0)
    // Konsumiere alle aufeinanderfolgenden Terminatoren (CR/LF/NULL-Cluster)
    bool consumed_any_terminator = false;
    while (rb->count > 0) {
        char ch = rb->buffer[rb->tail];
        if (!rb_is_line_term(ch)) break;
        rb->tail = rb_next(rb->tail, rb->size);
        rb->count--;
        consumed_any_terminator = true;
    }

    // Eine komplette Zeile wurde gelesen -> line_count einmal dekrementieren
    if (consumed_any_terminator && rb->line_count > 0) {
        rb->line_count--;
    }

    // NUL-terminieren
    out[copied] = '\0';

    if (rb->write_unlock) rb->write_unlock();
    return true;
}


