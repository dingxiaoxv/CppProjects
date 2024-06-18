
#ifndef _CNR_SOCKADDR_H_20141209_
#define _CNR_SOCKADDR_H_20141209_

#include <netinet/in.h>
#include <sys/un.h>

#include "cnr/mstream.h"

namespace cnr {

// POD
class SockAddr {
 public:
  SockAddr();

  explicit SockAddr(uint16_t port);

  SockAddr(uint32_t ip, uint16_t port);

  SockAddr(const char* ip, uint16_t port) : len_(0) { set(ip, port); }

  SockAddr(const char* udsName);

  // explicit SockAddr(const struct sockaddr_in& addr) : addr_(addr) {}

  bool operator==(const SockAddr&);

  void operator=(const SockAddr& other) { addr_ = other.addr_; }

  int set(const char* ip, uint16_t port);

  void set(uint32_t ip, uint16_t port);

  // const struct sockaddr_in& getSockAddrInet() const {return addr_;}

  // void setSockAddrInet(const struct sockaddr_in& addr) {addr_ = addr;}

  int len() const { return len_; }

  MStream toString();

  void toString(MStream& ms);

  static MStream toString(uint32_t ip, uint16_t port);

  static void toString(uint32_t ip, uint16_t port, MStream& ms);

  static MStream toString(uint32_t ip);

  static void toString(uint32_t ip, MStream& ms);

  // uint32_t getip() const {return addr_.sin_addr.s_addr;}

  // uint16_t getport() const {return ntohs(addr_.sin_port);}

 private:
  union {
    sockaddr_in v4_;
    sockaddr_in6 v6_;
    sockaddr_un un_;
  } addr_;
  int len_;
};

}  // namespace cnr

#endif  // _CNR_SOCKADDR_H_20141209_