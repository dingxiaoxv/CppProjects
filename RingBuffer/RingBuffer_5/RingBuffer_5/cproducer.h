#ifndef CPRODUCER_H
#define CPRODUCER_H

#include <QObject>
#include <QRunnable>

class CProducer : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit CProducer(QObject *parent = nullptr);

signals:

public:
    void run() override;

};

#endif // CPRODUCER_H
