#ifndef _CNR_CONDITION_H_20140323_
#define _CNR_CONDITION_H_20140323_

#include "cnr/mutex.h"

namespace cnr {

class Condition {
  DISALLOW_COPY_AND_ASSIGN(Condition);
  Mutex mutex_;
  pthread_cond_t c_;
  bool siged_;

 public:
  Condition() : siged_(false) {
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
    pthread_cond_init(&c_, &attr);
    pthread_condattr_destroy(&attr);
  }

  ~Condition() { pthread_cond_destroy(&c_); }

  void wait();

  bool wait(unsigned long msecs);

  void notify();

  void reset();
};

}  // namespace cnr

#endif  // _CNR_CONDITION_H_20140323_