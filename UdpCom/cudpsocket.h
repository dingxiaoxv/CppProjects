#ifndef _CUPDSOCKET_H
#define _CUPDSOCKET_H

#include <string>

class CUdpSocket {
public:
    CUdpSocket();
    ~CUdpSocket();

    bool Initialize(int port);
    bool Send(const std::string& data, const std::string& ipAddress, int port);
    bool Receive(std::string& data, std::string& ipAddress, int& port);
};

#endif