#ifndef _CNR_EVENT_WAITER_H_20221113_
#define _CNR_EVENT_WAITER_H_20221113_


#include <poll.h>

namespace cnr {

class EventWaiter {
public:
  EventWaiter() {
    fds_[0].fd = -1;
    fds_[0].events = 0;
  }

  EventWaiter(int fd) {
    fds_[0].fd = fd;
    fds_[0].events = POLLIN;
  }

  void setImcommingEvent(int fd) {
    fds_[0].fd = fd;
    fds_[0].events = POLLIN;
  }

  bool wait(int timeout) {
    if (fds_[0].events == 0) return false;
    return poll(fds_, 1, timeout) > 0;
  }

private:
  pollfd fds_[1];
};

} // namespace cnr


#endif // _CNR_EVENT_WAITER_H_20221113_