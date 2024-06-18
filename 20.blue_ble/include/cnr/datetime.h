#ifndef _CNR_DATE_H_20141114_
#define _CNR_DATE_H_20141114_

#include <chrono>
#include <ctime>
#include <iomanip>

#include "cnr/mstream.h"

namespace cnr {

class Datetime {
 public:
  Datetime() : valid_(false){};

  static Datetime now();
  static int currentEpoch();

  static int now(MStream& ms);
  static int nowWithOutUsec(MStream& ms);

  static int32_t ticks();

  static int32_t secTicks();

  int weekDay() const { return wday_; }
  time_t epoch() const { return epoch_; }

 public:
  bool valid_;
  int year_;
  int mon_;
  int day_;
  int hour_;
  int min_;
  int sec_;
  int usec_;
  int wday_;
  time_t epoch_;
};

}  // namespace cnr

#endif  // _CNR_DATE_H_20141114_