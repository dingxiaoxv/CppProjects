#include <string.h>
#include "ringbuffer.h"

void initRingBuffer(RING_BUFFER &rb, unsigned int sz)
{
    rb.size = sz;
    rb.readNum = 0;
    rb.writeNum = 0;
    rb.buff = new unsigned char[sz];
}

void deleteRingBuffer(RING_BUFFER &rb)
{
    if(rb.buff != nullptr)
    {
        delete[] rb.buff;
        rb.buff = nullptr;
    }
}

bool ringBufferIsEmpty(const RING_BUFFER &rb)
{
    return rb.writeNum == rb.readNum;
}

bool ringBufferIsFull(const RING_BUFFER &rb)
{
    return rb.writeNum == (rb.readNum + rb.size);
}

int writeToRingBuffer(RING_BUFFER &rb, unsigned char *szData, int iLen)
{
    if (ringBufferIsFull(rb))
    {
        return -1;
    }

    int available = rb.size - (rb.writeNum - rb.readNum);
    if (iLen > available)
    {
        return -2;
        //iLen = available;
    }

    unsigned int writeIndex = rb.writeNum % rb.size;
    if ((writeIndex + iLen) >= rb.size)
    {
        unsigned int len1 = rb.size - writeIndex;
        unsigned int len2 = iLen - len1;
        memcpy(rb.buff + writeIndex, szData, len1);
        memcpy(rb.buff, szData + len1, len2);
    }
    else
    {
        memcpy(rb.buff + writeIndex, szData, iLen);
    }
    rb.writeNum += iLen;
    // for (int i = 0; i < iLen; ++i)
    // {
    //     rb.buff[rb.writeNum % rb.size] = szData[i];
    //     ++rb.writeNum;
    // } 
    return 0;
}

int readFromRingBuffer(RING_BUFFER &rb, unsigned char *szData, int &iLen)
{
    if (ringBufferIsEmpty(rb))
    {
        return -1;
    }

    int available = rb.writeNum - rb.readNum;
    if (iLen > available)
    {
        iLen = available;
    }

    unsigned int readIndex = rb.readNum % rb.size;
    if ((readIndex + iLen) >= rb.size)
    {
        unsigned int len1 = rb.size - readIndex;
        unsigned int len2 = iLen - len1;
        memcpy(szData, rb.buff + readIndex, len1);
        memcpy(szData + len1, rb.buff, len2);
    }
    else
    {
        memcpy(szData, rb.buff + readIndex, iLen);
    }
    rb.readNum += iLen;
    // for (int i = 0; i < iLen; ++i)
    // {
    //     szData[i] = rb.buff[rb.readNum % rb.size];
    //     ++rb.readNum;
    // }   
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