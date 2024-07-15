#ifndef _CNR_EVENTBASE_H_
#define _CNR_EVENTBASE_H_

#include <map>

#include "cnr/cnr.h"
#include "cnr/condition.h"
#include "cnr/event.h"
#include "cnr/heap.h"

namespace cnr {

class EventDispather {
  DISALLOW_COPY_AND_ASSIGN(EventDispather);

 public:
  EventDispather();
  ~EventDispather();
  bool wakeup();
  bool stop();
  void remove(EventPtr ev);
  void remove(int fd);
  bool add(EventPtr spev);
  bool dispatch();
  void updateTimeoutHeap();
  int count() const { return events_.size(); }

 private:
  void removeTimeouted();

  void add(Event* ev, int events);

  void del(Event* ev, int events);
  void handleEvent(EventPtr spev, uint32_t events);

 private:
  int epoll_fd_;
  bool running_;
  int dispatched_;
  Condition exit_cond_;
  EventPtr wakeup_event_;
  std::map<int, EventPtr> events_;
};

}  // namespace cnr

#endif  // _CNR_EVENTBASE_H_