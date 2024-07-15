#ifndef _CNR_MUTEX_H_20140430_
#define _CNR_MUTEX_H_20140430_

#include <pthread.h>

#include "cnr/cnr.h"

namespace cnr {

class Mutex {
  DISALLOW_COPY_AND_ASSIGN(Mutex);

 public:
  Mutex(bool recu = false) {
    pthread_mutex_init(&m_, NULL);
    if (recu) {
      pthread_mutexattr_t attr;
      pthread_mutexattr_init(&attr);
      pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
      pthread_mutex_init(&m_, &attr);
      pthread_mutexattr_destroy(&attr);
    }
  }

  ~Mutex() { pthread_mutex_destroy(&m_); }

  // http://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_mutex_lock.html
  void lock() { pthread_mutex_lock(&m_); }

  void unlock() { pthread_mutex_unlock(&m_); }

  pthread_mutex_t* raw() { return &m_; }

 private:
  pthread_mutex_t m_;
};

class MutexGuard {
  DISALLOW_COPY_AND_ASSIGN(MutexGuard);
  Mutex& mutex_;

 public:
  MutexGuard(Mutex& mutex) : mutex_(mutex) { mutex_.lock(); }

  ~MutexGuard() { mutex_.unlock(); }
};

}  // namespace cnr

#endif  // _CNR_MUTEX_H_20140430_