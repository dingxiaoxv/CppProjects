#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <deque>
#include <mutex>
#include <condition_variable>

extern std::deque<std::string> g_dataQueue;
extern std::mutex g_dataMutex;
extern std::condition_variable g_dataCondition;

#endif
