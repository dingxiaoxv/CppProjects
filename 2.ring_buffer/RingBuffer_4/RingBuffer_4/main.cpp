#include <QCoreApplication>
#include <QDebug>
#include "ringbuffer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    RING_BUFFER stRingBuffer;
    initRingBuffer(stRingBuffer, 10);

    srand(time(0));
    int iwNum = rand() % stRingBuffer.size + 1;
    char *szWriteData = new char[iwNum];
    for (int j = 0; j < iwNum; ++j)
    {
        szWriteData[j] = j;
    }
    if (writeToRingBuffer(stRingBuffer, szWriteData, iwNum) < 0)
    {
        qDebug() << "ringBufferIsFull";
    }
    else
    {
        qDebug() << printRingBuffer(stRingBuffer);
    }
    delete[] szWriteData;

    for (int i = 0; i < 10; ++i)
    {
//        srand(time(0));
//        int iwNum = rand() % stRingBuffer.size + 1;
//        char *szWriteData = new char[iwNum];
//        for (int j = 0; j < iwNum; ++j)
//        {
//            szWriteData[j] = j;
//        }
//        if (writeToRingBuffer(stRingBuffer, szWriteData, iwNum) < 0)
//        {
//            qDebug() << "ringBufferIsFull";
//        }
//        else
//        {
//            qDebug() << printRingBuffer(stRingBuffer);
//        }
//        delete[] szWriteData;

        srand(time(0));
        int irNum = rand() % stRingBuffer.size + 1;
        QByteArray arr;
        if (readFromRingBuffer(stRingBuffer, arr, irNum) < 0)
        {
            qDebug() << "ringBufferIsEmpty";
        }
        else
        {
            QString str;
            for(auto it = arr.cbegin(); it != arr.cend(); ++it)
            {
                str += QString("%1 ").arg((quint8)(*it), 2, 16, QLatin1Char('0'));
            }
            qDebug() << str;
        }
    }

    return a.exec();
}
