#ifndef _CNR_OFSTREAM_H_20140503_
#define _CNR_OFSTREAM_H_20140503_

#include "cnr/fstream.h"

namespace cnr {

class OFStream : public FStream {
 public:
  OFStream() {}

  OFStream(const char* filename, bool is_append = false);

  ~OFStream() {}

  bool reopen(const char* filename, bool is_append = false);

  long length();

  bool flush() const { return fflush(file_) != 0; }

  bool flush_unlocked() const { return fflush_unlocked(file_) != 0; }

  bool write_unlocked(const char* buf) const { return fputs_unlocked(buf, file_) != 0; }

  size_t write_unlocked(const void* ptr, size_t objsize, size_t nobj = 1) const {
    return fwrite_unlocked(ptr, objsize, nobj, file_) * objsize;
  }

  bool write(const char* buf) const { return fputs(buf, file_) != 0; }

  size_t write(const void* ptr, size_t objsize, size_t nobj = 1) const {
    return fwrite(ptr, objsize, nobj, file_) * objsize;
  }
};

}  // namespace cnr

#endif  // _CNR_OFSTREAM_H_20140503_