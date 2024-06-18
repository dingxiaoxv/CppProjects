#ifndef _CNR_MSTREAM_H_20140504_
#define _CNR_MSTREAM_H_20140504_

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "cnr/error.h"

namespace cnr {

size_t hex2str(unsigned long addr, char* buf);

template <int _len, typename T>
size_t num2str(T t, char* buf);

class MStream {
 public:
  enum NumType {
    DEC = 10,
    HEX = 16,
    HEX_NOPREFIX = 17,
  };
  MStream(size_t size = 1024);

  explicit MStream(char* buf, size_t size,  bool logms = false)
      : size_(size), buf_(buf), pos_(buf), on_heap_(false), numtype_(DEC), logms_(logms) {
    buf_[0] = 0;
  }

  explicit MStream(char* buf, size_t len, size_t cap)
      : size_(cap), buf_(buf), pos_(buf + len), on_heap_(false), numtype_(DEC), logms_(false) {}

  explicit MStream(const char* buf);

  ~MStream() {
    if (on_heap_) delete[] buf_;
  }

  bool startswith(const char* str) const;
  bool endswith(const char* str) const;

  MStream(const MStream& ms);

  MStream& operator=(const MStream& ms);

  size_t available() const { return size_ - (pos_ - buf_); }

  // unsafe, it's hard for me to check is there enough buffer
  int fmt(const char* format, ...);

  const char* data() const { return buf_; }

  void reset() { pos_ = buf_; }

  bool empty() const { return pos_ == buf_ ? true : false; }

  bool write(FILE* fp) const { return fputs(buf_, fp) == 0 ? true : false; }  // FIXME??

  // we add '\0' and the end, so the length need't contain '\0'
  inline bool append(const char* buf, size_t len) {
    if (available() < len + 1) {
      return false;
    }

    memcpy(pos_, buf, len);
    pos_ += len;
    *pos_ = '\0';
    return true;
  }

  inline bool append(const char* buf) { return append(buf, strlen(buf)); }

  void swap(MStream& ms) {
    size_t tmp = size_;
    size_ = ms.size_;
    ms.size_ = tmp;

    char* tmpBuf = buf_;
    buf_ = ms.buf_;
    ms.buf_ = tmpBuf;

    char* tmpPos = pos_;
    pos_ = ms.pos_;
    ms.pos_ = tmpPos;
  }

  inline MStream& operator<<(const char* buf) {
    CHECK_FAIL(append(buf) || logms_, "no enough buf");
    return *this;
  }

  MStream& operator<<(char c);

  inline MStream& operator<<(const void* p) {
    pos_ += num2str<16, size_t>((size_t)p, pos_);
    return *this;
  }

  inline MStream& operator<<(const std::string& str) {
    CHECK_FAIL(append(str.c_str()) || logms_, "no engough buf");
    return *this;
  }

  inline MStream& operator<<(NumType t) {
    numtype_ = t;
    return *this;
  }

  inline MStream& operator<<(int num) { return appendNum(num); }

  inline MStream& operator<<(unsigned int num) { return appendNum(num); }

  inline MStream& operator<<(short num) { return appendNum(num); }

  inline MStream& operator<<(unsigned short num) { return appendNum(num); }

  inline MStream& operator<<(long num) { return appendNum(num); }

  inline MStream& operator<<(unsigned long num) { return appendNum(num); }

  inline MStream& operator<<(float num) {
    size_t sizeAva = available();
    pos_ += snprintf(pos_, sizeAva, "%f", num);
    return *this;
  }

  inline MStream& operator<<(double num) {
    size_t sizeAva = available();
    pos_ += snprintf(pos_, sizeAva, "%f", num);
    return *this;
  }

  inline MStream& operator<<(const MStream& ms) { return operator<<(ms.buf_); }

  size_t size() const { return pos_ - buf_; }

  bool equal(const char* str) const;

  bool equal(const MStream& ms) const;
  std::vector<std::string> split(const char* sep) const;

 private:
  template <typename T>
  MStream& appendNum(T t) {
    CHECK_FAIL(available() > kMax, "No available buffer for you.");
    switch (numtype_) {
      case DEC:
        pos_ += num2str<DEC>(t, pos_);
        break;

      case HEX:
        pos_ += num2str<HEX>(t, pos_);
        break;

      case HEX_NOPREFIX:
        pos_ += num2str<HEX_NOPREFIX>(t, pos_);
        break;
    }

    *pos_ = '\0';

    return *this;
  }

 private:
  friend class Logging;
  size_t size_;
  char* buf_;
  char* pos_;
  bool on_heap_;
  NumType numtype_;
  bool logms_;

  static const size_t kMax = 24;
};

template <int _len, typename T>
size_t num2str(T t, char* buf) {
  const char numbers[] = "FEDCBA9876543210123456789ABCDEF";
  const char* const zero = numbers + 0xF;
  char* pos = buf;
  int len = _len;
  if (_len == cnr::MStream::HEX_NOPREFIX) len = cnr::MStream::HEX;

  T t2 = t;
  do {
    int i = t % len;
    t /= len;
    *pos++ = zero[i];
  } while (t != 0);

  if (_len == static_cast<int>(cnr::MStream::HEX)) {
    *pos++ = 'x';
    *pos++ = '0';
  }

  if (t2 < 0) {
    *pos++ = '-';
  }

  std::reverse(buf, pos);
  return pos - buf;
}

}  // namespace cnr

#define LOCAL_MS(size, name) \
  char _1_buf[size];         \
  cnr::MStream name(_1_buf, size)

#endif  // _CNR_MSTREAM_H_20140504_