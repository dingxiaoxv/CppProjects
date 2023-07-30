#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <iostream>
#include <string>

typedef struct
{
    unsigned int size;
    unsigned int writeIndex;
    unsigned int readIndex;
    unsigned int buffCount;
    unsigned char *buff = nullptr;
} RING_BUFFER;

void initRingBuffer(RING_BUFFER &rb, unsigned int sz);
void deleteRingBuffer(RING_BUFFER &rb);
bool ringBufferIsEmpty(const RING_BUFFER &rb);
bool ringBufferIsFull(const RING_BUFFER &rb);
int writeToRingBuffer(RING_BUFFER &rb, unsigned char *szData, int iLen);
int readFromRingBuffer(RING_BUFFER &rb, unsigned char *szData, int &iLen);
void printRingBuffer(const RING_BUFFER &rb);
std::string charToHexString(const char &ch);

#endif