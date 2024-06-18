#ifndef _CNR_EVENT_H_20150820_
#define _CNR_EVENT_H_20150820_

#include <sys/epoll.h>

#include <functional>
#include <memory>

#include "cnr/socket.h"

#define CNR_EV_READ_ONLY (EPOLLIN | EPOLLERR | EPOLLET)
#define CNR_EV_WRITE_ONLY (EPOLLOUT | EPOLLERR | EPOLLET)
#define CNR_EV_RW (EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLET)

namespace cnr {

class Event;

typedef std::shared_ptr<Event> EventPtr;
typedef std::function<void(Event* ev)> EvRWCallback;

struct EventBuffer {
  char* buffer;
  int len;
  int pos;

  EventBuffer() : buffer(nullptr), len(0), pos(0) {}
  EventBuffer(int _len) : buffer(new char[_len]), len(_len), pos(0) {}
  ~EventBuffer() {
    if (buffer) delete[] buffer;
  }
  void reset(int _len);
  void processed(int len);
};

class Event {
 public:
  Event(int fd, int events, EvRWCallback rcb = nullptr, EvRWCallback wcb = nullptr);
  static EventPtr timerEvent(int msec, bool one_shot, EvRWCallback callback);
  static EventPtr efdEvent(EvRWCallback callback);

  Event();
  ~Event();
  int fd() const { return fd_; }
  void setEvents(int events) { events_ = events; }
  void setCallbacks(EvRWCallback rcb, EvRWCallback wcb);
  void doRemove() { remove_ = true; }
  int read(void* buf, size_t len);
  bool read64();
  int write(const void* buf, size_t len);
  int write(const char* buf) { return write(buf, strlen(buf)); }
  void resetReadBuffer(uint32_t len) { read_buffer_.reset(len); }
  int read();
  EventBuffer& getReadBuffer() { return read_buffer_; }

 private:
  friend class EventDispather;
  void handle(int events);
  bool needRemove() const { return remove_; }

 private:
  int fd_;
  int events_;
  EvRWCallback rcb_;
  EvRWCallback wcb_;
  bool remove_;
  EventBuffer read_buffer_;
  EventBuffer write_buffer_;
};

}  // namespace cnr

#endif  // _CNR_EVENT_H_20150820_