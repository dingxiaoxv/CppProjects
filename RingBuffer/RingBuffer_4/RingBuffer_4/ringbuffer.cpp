#include "ringbuffer.h"
#include <QString>
#include <QMutex>

QMutex mutex;

void initRingBuffer(RING_BUFFER &rb, unsigned int sz)
{
    rb.size = sz;
    rb.readNum = 0;
    rb.writeNum = 0;
    rb.buff.resize(sz);
}

bool ringBufferIsEmpty(const RING_BUFFER &rb)
{
    return rb.writeNum == rb.readNum;
}

bool ringBufferIsFull(const RING_BUFFER &rb)
{
    return rb.writeNum == (rb.readNum + rb.size);
}

int writeToRingBuffer(RING_BUFFER &rb, char *szData, int iLen)
{
    if (ringBufferIsFull(rb))
    {
        return -1;
    }

    mutex.lock();
    int available = rb.size - (rb.writeNum - rb.readNum);
    mutex.unlock();
    if (iLen > available)
    {
        //return -2;
        iLen = available;
    }

    QByteArray arr(szData, iLen);
    for(auto it = arr.cbegin(); it != arr.cend(); ++it)
    {
        rb.buff[rb.writeNum % rb.size] = *it;
        ++rb.writeNum;
    }

    return 0;
}

int readFromRingBuffer(RING_BUFFER &rb, QByteArray &arr, int &iLen)
{
    if (ringBufferIsEmpty(rb))
    {
        return -1;
    }

    mutex.lock();
    int available = rb.writeNum - rb.readNum;
    mutex.unlock();
    if (iLen > available)
    {
        iLen = available;
    }

    for (int i = 0; i < iLen; ++i)
    {
        arr.push_back(rb.buff.at(rb.readNum % rb.size));
        ++rb.readNum;
    }
    return 0;
}

QString printRingBuffer(const RING_BUFFER &rb)
{
    QString msg;
    for (auto it = rb.buff.cbegin(); it != rb.buff.cend(); ++it)
    {
       msg += QString("%1 ").arg((quint8)(*it), 2, 16, QLatin1Char('0'));
    }
    return msg;
}
