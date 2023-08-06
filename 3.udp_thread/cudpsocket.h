#ifndef _CUPDSOCKET_H
#define _CUPDSOCKET_H

#include <string>

const int BUFFER_SIZE = 1024;
class CUdpSocket
{
public:
    CUdpSocket(int port);
    ~CUdpSocket();
    int send(const std::string &data, const std::string &ipAddress, int port);
    int receive(std::string &data, std::string &ipAddress, int &port);

private:
    int m_socket;
};

#endif