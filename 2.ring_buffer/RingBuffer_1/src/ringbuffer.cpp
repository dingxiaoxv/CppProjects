#include <string.h>
#include "ringbuffer.h"

void initRingBuffer(RING_BUFFER &rb, unsigned int sz)
{
    rb.size = sz;
    rb.readIndex = 0;
    rb.writeIndex = 0;
    if (rb.buff == nullptr)
    {
        rb.buff = new unsigned char[sz]{'\0'};
    }
}

void deleteRingBuffer(RING_BUFFER &rb)
{
    if (rb.buff != nullptr)
    {
        delete[] rb.buff;
        rb.buff = nullptr;
    }
}

bool ringBufferIsEmpty(const RING_BUFFER &rb)
{
    return rb.writeIndex == rb.readIndex;
}

bool ringBufferIsFull(const RING_BUFFER &rb)
{
    return ((rb.writeIndex + 1) % rb.size) == (rb.readIndex % rb.size);
}

int writeToRingBuffer(RING_BUFFER &rb, unsigned char *szData, int iLen)
{
    if (ringBufferIsFull(rb))
    {
        return -1;
    }

    int available = 0;
    if (rb.writeIndex >= rb.readIndex)
    {
        available = rb.size - (rb.writeIndex - rb.readIndex) - 1;
    }
    else
    {
        available = rb.readIndex - rb.writeIndex - 1;
    }
    if (iLen > available)
    {
        // return -2;
        iLen = available;
    }

    if ((rb.writeIndex + iLen) >= rb.size)
    {
        unsigned int len1 = rb.size - rb.writeIndex;
        unsigned int len2 = iLen - len1;
        memcpy(rb.buff + rb.writeIndex, szData, len1);
        memcpy(rb.buff, szData + len1, len2);
        rb.writeIndex = len2;
    }
    else
    {
        memcpy(rb.buff + rb.writeIndex, szData, iLen);
        rb.writeIndex += iLen;
    }
    return 0;
}

int readFromRingBuffer(RING_BUFFER &rb, unsigned char *szData, int &iLen)
{
    if (ringBufferIsEmpty(rb))
    {
        return -1;
    }

    int available = 0;
    if (rb.writeIndex >= rb.readIndex)
    {
        available = rb.writeIndex - rb.readIndex;
    }
    else
    {
        available = rb.size - (rb.readIndex - rb.writeIndex);
    }
    if (iLen > available)
    {
        iLen = available;
    }

    if ((rb.readIndex + iLen) >= rb.size)
    {
        unsigned int len1 = rb.size - rb.readIndex;
        unsigned int len2 = iLen - len1;
        memcpy(szData, rb.buff + rb.readIndex, len1);
        memcpy(szData + len1, rb.buff, len2);
        rb.readIndex = len2;
    }
    else
    {
        memcpy(szData, rb.buff + rb.readIndex, iLen);
        rb.readIndex += iLen;
    }
    return 0;
}

void printRingBuffer(const RING_BUFFER &rb)
{
    for (size_t i = 0; i < rb.size; ++i)
    {
        std::cout << charToHexString(rb.buff[i]) << " ";
    }
    std::cout << std::endl;
}

std::string charToHexString(const char &ch)
{
    std::string ret = "0x";
    const char *hex = "0123456789ABCDEF";
    ret.push_back(hex[(ch >> 4) & 0xf]); //取二进制高四位
    ret.push_back(hex[ch & 0xf]);        //取二进制低四位
    return ret;
}