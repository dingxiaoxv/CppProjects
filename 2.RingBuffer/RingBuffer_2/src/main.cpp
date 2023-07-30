#include <iostream>
#include <cstdlib>
#include <ctime>
#include "ringbuffer.h"

int main()
{
    RING_BUFFER stRingBuffer;
    initRingBuffer(stRingBuffer, 10);

    // srand(time(0));
    // int iwNum = rand() % stRingBuffer.size + 1;
    // unsigned char *szWriteData = new unsigned char[iwNum];
    // for (int j = 0; j < iwNum; ++j)
    // {
    //     szWriteData[j] = j;
    // }
    // if (writeToRingBuffer(stRingBuffer, szWriteData, iwNum) < 0)
    // {
    //     std::cout << "ringBufferIsFull" << std::endl;
    // }
    // else
    // {
    //     printRingBuffer(stRingBuffer);
    // }
    // delete[] szWriteData;

    for (int i = 0; i < 10; ++i)
    {
        srand(time(0));
        int iwNum = rand() % stRingBuffer.size + 1;
        unsigned char *szWriteData = new unsigned char[iwNum];
        for (int j = 0; j < iwNum; ++j)
        {
            szWriteData[j] = j;
        }
        if (writeToRingBuffer(stRingBuffer, szWriteData, iwNum) < 0)
        {
            std::cout << "ringBufferIsFull" << std::endl;
        }
        else
        {
            printRingBuffer(stRingBuffer);
        }
        delete[] szWriteData;

        srand(time(0));
        int irNum = rand() % stRingBuffer.size + 1;
        unsigned char *szReadData = new unsigned char[irNum];
        if (readFromRingBuffer(stRingBuffer, szReadData, irNum) < 0)
        {
            std::cout << "ringBufferIsEmpty" << std::endl;
        }
        else
        {
            for (int k = 0; k < irNum; ++k)
            {
                std::cout << charToHexString(szReadData[k]) << " ";
            }
            std::cout << std::endl;
        }
        delete[] szReadData;
    }
    deleteRingBuffer(stRingBuffer);

    return 0;
}