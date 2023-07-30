#ifndef CCONSUMER_H
#define CCONSUMER_H

#include <QObject>
#include <QRunnable>

class CConsumer : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit CConsumer(QObject *parent = nullptr);

signals:
    void consumeCompleted();

public:
    void run() override;

private slots:
    void stopConsume();

private:
    bool m_stopflag = false;
    const quint16 CONSUME_DATASIZE = 20;
};

#endif // CCONSUMER_H
