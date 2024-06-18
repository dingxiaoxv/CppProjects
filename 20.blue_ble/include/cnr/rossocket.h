#ifndef _CNR_ROS_SOCKET_20240125_
#define _CNR_ROS_SOCKET_20240125_

#include "cnr/socket.h"
#include "cnr/mutex.h"
#include "cnr/condition.h"
#include <ros/callback_queue.h>
#include <poll.h>
#include <vector>

namespace cnr {

class RosSocket : public ros::CallbackInterface {
public:
  RosSocket(Socket::Type type,
            std::function<void(const char* buffer, int len)> on_data = nullptr,
            std::function<void()> on_out = nullptr,
            std::function<void()> on_incomming = nullptr);
  virtual ~RosSocket() = default;

  virtual CallResult call();
  virtual bool ready() { return ready_; }

  cnr::Socket& socket() { return socket_; }
  void resetSocket();

private:
  friend class RosSocketManager;
  cnr::Socket socket_;
  char buffer_[4096];
  bool to_close_ {false};
  bool ready_ {false};
  std::atomic<uint32_t> events_ {0};
  std::function<void(const char* buffer, int len)> on_data_;
  std::function<void()> on_out_;
  std::function<void()> on_incomming_;
};

class RosSocketManager {
public:
  static RosSocketManager& instance();
  ~RosSocketManager();

  void addSocket(boost::shared_ptr<RosSocket> socket);
  void removeSocket(boost::shared_ptr<RosSocket> socket);

private:
  RosSocketManager();
  void run();
  void addEpollEvent(int fd, int events);
  void removeEpollEvent(int fd);

private:
  int epoll_fd_;
  int wakeup_fd_;
  std::map<int, boost::shared_ptr<RosSocket>> sockets_;
  cnr::Mutex mutex_;
  cnr::Condition startup_cond_;
};

}  // namespace cnr

#endif  // _CNR_ROS_SOCKET_20240125_