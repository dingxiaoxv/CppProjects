#ifndef _CNR_HEAP_H_
#define _CNR_HEAP_H_

#include <algorithm>
#include <vector>

#include "cnr/cnr.h"

namespace cnr {

template <typename T, typename Compare = std::less<T>>
class Heap {
  DISALLOW_COPY_AND_ASSIGN(Heap);
  std::vector<T> data_;

 public:
  typedef std::vector<T> DataType;

  Heap() {}

  ~Heap() {}
  Heap(const DataType& vt, bool use_swap) {
    if (use_swap)
      data_.swap(vt);
    else
      data_ = vt;
  }

  Heap(const T* arr, size_t len) : data_(arr, arr + len - 1) {}

  const DataType& data() const { return data_; }

  bool empty() const { return data_.empty(); }

  size_t size() const { return data_.size(); }

  T root() { return data_[0]; }

  void push(T t) {
    data_.push_back(t);
    std::push_heap(data_.begin(), data_.end(), Compare());
  }

  bool pop() {
    if (data_.empty()) return false;

    std::pop_heap(data_.begin(), data_.end(), Compare());
    data_.pop_back();

    return true;
  }

  bool update() {
    if (data_.empty()) return false;
    std::make_heap(data_.begin(), data_.end(), Compare());
    return true;
  }
};

}  // namespace cnr

#endif  // _CNR_HEAP_H_