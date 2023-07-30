#include "cconsumer.h"
#include "global.h"
#include <iostream>

CConsumer::CConsumer(QObject *parent)
    : QObject{parent}, QRunnable()
{
    setAutoDelete(true);
    connect(this, &CConsumer::consumeCompleted, this, &CConsumer::stopConsume);
}

void CConsumer::run()
{
    while(!m_stopflag)
    {
        //static quint32 num = 0;
        quint16 len = 0;
        if((usedBytes.available() - CONSUME_DATASIZE) >= 0)
        {
            len = CONSUME_DATASIZE;
        }
        else
        {
            len = (usedBytes.available() == 0)? 1 : usedBytes.available();
        }
        usedBytes.acquire(len);

        g_mutex.lock();
        std::cout << "=========" << "consumer" << "=========" << std::endl;
        std::cout << "consumer acq: " << len << " usedBytes: " << usedBytes.available() << std::endl;
        for (auto i = readNum; i < readNum + len; ++i)
        {
            std::cout << charToHexString(g_szBuffer[i % BUFFERSIZE]) << " ";
        }
        std::cout << std::endl;
        readNum += len;
        freeBytes.release(len);
        std::cout << "consumer rel: " << len << " freeBytes: " << freeBytes.available() << std::endl;
        g_mutex.unlock();

        if(readNum >= DATASIZE)
        {
            emit consumeCompleted();
        }
    }
}

void CConsumer::stopConsume()
{
    m_stopflag = true;
}
