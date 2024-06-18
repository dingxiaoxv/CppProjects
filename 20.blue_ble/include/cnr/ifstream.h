#ifndef _CNR_IFSTREAM_H_20140503_
#define _CNR_IFSTREAM_H_20140503_

#include "cnr/fstream.h"

namespace cnr {

class IFStream : public FStream {
 public:
  IFStream() {}

  IFStream(const char* filename);

  ~IFStream() {}

  bool reopen(const char* filename);

  long length() const { return len_; }

 private:
  long len_;
};

}  // namespace cnr

#endif  // _CNR_IFSTREAM_H_20140503_