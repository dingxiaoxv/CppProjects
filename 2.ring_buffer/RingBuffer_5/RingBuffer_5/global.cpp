#include "global.h"

const quint32 DATASIZE = 20;
const quint32 BUFFERSIZE = 10;
QByteArray g_szBuffer(BUFFERSIZE, '\0');
QSemaphore freeBytes(BUFFERSIZE);
QSemaphore usedBytes;
quint32 writeNum = 0;
quint32 readNum = 0;
QMutex g_mutex;

std::string charToHexString(const char &ch)
{
    std::string ret = "0x";
    const char *hex = "0123456789ABCDEF";
    ret.push_back(hex[(ch >> 4) & 0xf]); //取二进制高四位
    ret.push_back(hex[ch & 0xf]);        //取二进制低四位
    return ret;
}
