#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> // For memset, if buffer is static

// Configuration: Choose dynamic or static buffer size
#define DYNAMIC_BUFFERSIZE 0  // 1: dynamic, 0: static

// Default size for the static buffer
#define STATIC_BUFFER_SIZE 256

typedef void (*rb_hook_t)(void);

// RingBuffer data structure
typedef struct {
#if DYNAMIC_BUFFERSIZE == 1
    char *buffer;       // Dynamic buffer (when DYNAMIC_BUFFERSIZE is 1)
#else
    char buffer[STATIC_BUFFER_SIZE]; // Static buffer (when DYNAMIC_BUFFERSIZE is 0)
#endif
    uint16_t head;        // Pointer to the head
    uint16_t tail;        // Pointer to the tail
    uint16_t count;       // Number of stored elements
    uint16_t size;        // Buffer size
    volatile uint16_t line_count;
    // Hooks
    rb_hook_t write_lock;
    rb_hook_t write_unlock;
    rb_hook_t read_lock;
    rb_hook_t read_unlock;

} RingBuffer;

// RingBuffer API
#if DYNAMIC_BUFFERSIZE == 1
void RingBuffer_Init(volatile RingBuffer *rb, uint16_t buffer_size);
#else
void RingBuffer_Init(volatile RingBuffer *rb);
#endif
void RingBuffer_SetWriteLock(volatile RingBuffer *rb, rb_hook_t hook);
void RingBuffer_SetWriteUnlock(volatile RingBuffer *rb, rb_hook_t hook);
void RingBuffer_SetReadLock(volatile RingBuffer *rb, rb_hook_t hook);
void RingBuffer_SetReadUnlock(volatile RingBuffer *rb, rb_hook_t hook);
void RingBuffer_Free(volatile RingBuffer *rb);
bool RingBuffer_IsFull(volatile RingBuffer *rb);
bool RingBuffer_IsEmpty(volatile RingBuffer *rb);
bool RingBuffer_Write(volatile RingBuffer *rb, char data);
bool RingBuffer_Read(volatile RingBuffer *rb, char *data);
bool RingBuffer_WriteString(volatile RingBuffer *rb, const char *str);
bool RingBuffer_ReadLine(volatile RingBuffer* rb, char* out, uint16_t outSize);
uint16_t RingBuffer_NumberOfLines(volatile RingBuffer* rb);


