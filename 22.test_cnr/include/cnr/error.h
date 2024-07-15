#ifndef _CNR_ERROR_H_201404061503_
#define _CNR_ERROR_H_201404061503_

#include "cnr/cnr.h"

namespace cnr {

class Error {
  DISALLOW_ASSIGN(Error);

 private:
  const char* description_;

 public:
  explicit Error(const char* _description) : description_(_description) {}

  const char* description() const { return description_; }
};

class ValueError : public Error {
 public:
  ValueError(const char* _description) : Error(_description) {}
};

class MissionError : public Error {
 public:
  MissionError(const char* _description) : Error(_description) {}
};

class SocketError : public Error {
 public:
  SocketError(const char* description) : Error(description) {}
};

class IOError : public Error {
 public:
  IOError(const char* description) : Error(description) {}
};
}  // namespace cnr

#ifdef _DEBUG
#define CHECK_ERROR(cond, desc)          \
  do {                                   \
    if (!(cond)) throw cnr::Error(desc); \
  } while (0)
#else
#define CHECK_ERROR(cond, desc)
#endif

#define CHECK_FAIL(cond, desc)           \
  do {                                   \
    if (!(cond)) throw cnr::Error(desc); \
  } while (0)

#endif  // _CNR_ERROR_H_201404061503_
