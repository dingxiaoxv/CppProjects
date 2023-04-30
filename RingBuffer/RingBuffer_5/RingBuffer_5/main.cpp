#include <QCoreApplication>
#include <QThreadPool>
#include <iostream>
#include "cproducer.h"
#include "cconsumer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 线程池初始化，设置最大线程池数
    QThreadPool::globalInstance()->setMaxThreadCount(4);
    // 添加任务
    CProducer *pProducer = new CProducer;
    CConsumer *pConsumer = new CConsumer;
    QThreadPool::globalInstance()->start(pProducer);
    QThreadPool::globalInstance()->start(pConsumer);

    std::cout << "mission complished!" << std::endl;

    return a.exec();
}
