#ifndef GLOBAL_H
#define GLOBAL_H

#include <QByteArray>
#include <QMutex>
#include <QSemaphore>

extern const quint32 DATASIZE;
extern const quint32 BUFFERSIZE;
extern QByteArray g_szBuffer;
extern QSemaphore freeBytes;
extern QSemaphore usedBytes;
extern quint32 writeNum;
extern quint32 readNum;
extern QMutex g_mutex;

extern std::string charToHexString(const char& ch);

#endif // GLOBAL_H
