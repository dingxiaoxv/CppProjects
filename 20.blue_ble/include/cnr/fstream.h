#ifndef _CNR_FSTREAM_H_20140503_
#define _CNR_FSTREAM_H_20140503_

#include <cstdio>

#include "cnr/cnr.h"

namespace cnr {

class FStream {
  DISALLOW_COPY_AND_ASSIGN(FStream);

 protected:
  FStream() : file_(nullptr) {}

 public:
  enum Seek {
    set = SEEK_SET,
    cur = SEEK_CUR,
    end = SEEK_END,
  };

  ~FStream() { close(); }

  void close();

  FILE* raw() { return file_; }

  bool readline(char* buf, int cnt);

  size_t read(void* buf, size_t objsize, size_t nobjs = 1);

  bool seek(long _offset, Seek seek_type) { return fseek(file_, _offset, seek_type) == 0; }

  bool eof() const { return feof(file_) > 0; }

  size_t pos() const { return ftell(file_); }

 protected:
  FILE* file_;
};

}  // namespace cnr

#endif  // _CNR_FSTREAM_H_20140503_