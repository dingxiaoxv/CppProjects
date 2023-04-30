  #include <vector>

  template <class T>
  class RingBuffer
  {
  public:
     RingBuffer(size_t buffer_size)
        : ring_(buffer_size)
        , buffer_size_(buffer_size)
        , head_(0)
        , tail_(0)
     {
     }

     T* back()
     {
        bool received = false;

        if(available(head_, tail_))
        {
           return &(ring_[head_ % buffer_size_]);
        }

        return nullptr;
     }

     void push()
     {
        ++head_;
     }

     T* front()
     {
        if(tail_ < head_)
        {
           return & ring_[tail_ % buffer_size_];
        }

        return nullptr;
     }

     void pop()
     {
        ++tail_;
     }

     size_t size() const
     {
        if(tail_ < head_)
           return buffer_size_ - ((tail_ + buffer_size_) - head_);
        else if(tail_ > head_)
           return buffer_size_ - (tail_ - head_);

        return 0;
     }

     bool available()
     {
        return available(head_, tail_);
     }

  private:
     bool available(uint64_t h, uint64_t t) const
     {
        if(h == t)
           return true;
        else if(t > h)
           return (t - h) > buffer_size_;
        else// if(h > t)
           return (t + buffer_size_) - h > 0;
     }

     std::vector<T> ring_;
     const size_t   buffer_size_;
     std::atomic<uint64_t> head_;
     std::atomic<uint64_t> tail_;
  };
