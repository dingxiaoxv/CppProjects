#include "cproducer.h"
#include "global.h"
#include <QtCore>
#include <iostream>
#include <string>

CProducer::CProducer(QObject *parent)
    : QObject{parent}, QRunnable()
{
    //任务执行完毕,该对象自动销毁
    setAutoDelete(true);
}

void CProducer::run()
{
    //static quint32 num = 0;
    while(writeNum < DATASIZE)
    {
        quint16 len = QRandomGenerator::global()->bounded(BUFFERSIZE) + 1;
        if(freeBytes.available() - len < 0)
        {
            len = freeBytes.available();
        }
        freeBytes.acquire(len);

        g_mutex.lock();
        std::cout << "=========" << "producer" << "=========" << std::endl;
        std::cout << "producer acq: " << len << " freeBytes: " << freeBytes.available() << std::endl;
        for (auto i = writeNum; i < writeNum + len; ++i)
        {
            g_szBuffer[i % BUFFERSIZE] = QRandomGenerator::global()->bounded(100);
            std::cout << charToHexString(g_szBuffer[i % BUFFERSIZE]) << " ";
        }
        writeNum += len;
        std::cout << std::endl;
        usedBytes.release(len);
        std::cout << "producer rel: " << len << " usedBytes: " << usedBytes.available() << std::endl;
        g_mutex.unlock();
    }
}
