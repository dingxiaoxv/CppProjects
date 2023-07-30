#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <iostream>
#include <string>
using namespace std;

typedef struct
{
    unsigned int size;
    unsigned int writeNum;
    unsigned int readNum;
    unsigned char *buff = nullptr;
} RING_BUFFER;

void initRingBuffer(RING_BUFFER &rb, unsigned int sz);
void deleteRingBuffer(RING_BUFFER &rb);
bool ringBufferIsEmpty(const RING_BUFFER &rb);
bool ringBufferIsFull(const RING_BUFFER &rb);
int writeToRingBuffer(RING_BUFFER &rb, unsigned char *szData, int iLen);
int readFromRingBuffer(RING_BUFFER &rb, unsigned char *szData, int &iLen);
void printRingBuffer(const RING_BUFFER &rb);
string charToHexString(const char &ch);

#endif