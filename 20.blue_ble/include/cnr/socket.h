#ifndef _CNR_SOCKET_H_20141209_
#define _CNR_SOCKET_H_20141209_

#include <netinet/tcp.h>
#include <memory>
#include "cnr/socketaddr.h"

namespace cnr {

class Event;

class Socket {
 public:
  enum Type {
    TCP = 0,
    UDP = 1,
    UDS = 2,
    UDG = 3,
    INV = 4,
  };

  Socket(int fd) : fd_(fd) {}
  explicit Socket(Type socketType = INV);
  // TODO: dtor
  Socket& operator=(int fd) {
    fd_ = fd;
    return *this;
  }

  int fd() const { return fd_; }
  int reusePort(int on) {
    return ::setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &on, static_cast<socklen_t>(sizeof on));
  }
  int setNoblock();
  int tcpNoDelay(int on) {
    return ::setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, (char*)&on, sizeof(int));
  }
  int keepAlive(int on) {
    return ::setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, (char*)&on, sizeof(int));
  }
  int reuseAddr(int on) {
    return ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(int));
  }
  int broadcast(int on) {
    return ::setsockopt(fd_, SOL_SOCKET, SO_BROADCAST, (char*)&on, sizeof(int));
  }
  int setRecvBufferSize(int size) {
    return ::setsockopt(fd_, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(int));
  }
  int bind(const SockAddr& localaddr) {
    return ::bind(fd_, (const sockaddr*)&localaddr, localaddr.len());
  }
  int listen(int n = 5) { return ::listen(fd_, n); }
  int connect(const SockAddr& servAddr) {
    return ::connect(fd_, (const sockaddr*)&servAddr, servAddr.len());
  }
  int connect(const char* strIp, uint16_t port);
  Socket accept(SockAddr& peeraddr);
  std::shared_ptr<Event> accept();
  bool valid() const { return fd_ != INVALID_SOCKET; }
  bool hasError() const;
  int recv(char* buf, size_t len) { return ::recv(fd_, buf, len, 0); }
  int send(const char* buf) { return send(buf, strlen(buf)); }
  int send(const char* buf, size_t len) { return ::send(fd_, buf, len, 0); }
  int shutdownWrite() { return ::shutdown(fd_, SHUT_WR); }
  int shutdownRead() { return ::shutdown(fd_, SHUT_RD); }
  int shutdownAll() { return ::shutdown(fd_, SHUT_RDWR); }
  int close();
  void reset(Type socketType = TCP);
  int recvfrom(char* buf, size_t len, SockAddr& peer) {
    socklen_t socklen = sizeof(peer);
    return ::recvfrom(fd_, buf, len, 0, (sockaddr*)&peer, &socklen);
  }
  int sendto(const char* buf, size_t len, SockAddr& peer) {
    return ::sendto(fd_, buf, len, 0, (sockaddr*)&peer, peer.len());
  }
  static Socket getListenSockets(Type type, const char* ip, const char* port);
  static void dump(MStream& ms, const uint8_t* buf, int len);

 private:
  int fd_;
};

}  // namespace cnr

#endif  // _CNR_SOCKET_H_20141209_