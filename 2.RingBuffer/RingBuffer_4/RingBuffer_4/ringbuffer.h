#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <QByteArray>

typedef struct
{
    unsigned int size;
    unsigned int writeNum;
    unsigned int readNum;
    QByteArray buff;
} RING_BUFFER;

void initRingBuffer(RING_BUFFER &rb, unsigned int sz);
bool ringBufferIsEmpty(const RING_BUFFER &rb);
bool ringBufferIsFull(const RING_BUFFER &rb);
int writeToRingBuffer(RING_BUFFER &rb, char *szData, int iLen);
int readFromRingBuffer(RING_BUFFER &rb, QByteArray &arr, int &iLen);
QString printRingBuffer(const RING_BUFFER &rb);

#endif // RINGBUFFER_H
