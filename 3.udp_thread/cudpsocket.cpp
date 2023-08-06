#include "cudpsocket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

CUdpSocket::CUdpSocket(int port)
{
    // Create socket
    m_socket = socket(AF_INET, SOCK_DGRAM, 0); // AF_INET -> IPv4
    if (m_socket < 0)
    {
        throw std::runtime_error("Failed to create UDP socket");
    }

    // Bind to the specified port
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(m_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        close(m_socket);
        throw std::runtime_error(std::string("Failed to bind port " + std::to_string(port)));
    }
}

CUdpSocket::~CUdpSocket()
{
    close(m_socket);
}

int CUdpSocket::send(const std::string &data, const std::string &ipAddress, int port)
{
    sockaddr_in destAddr{};
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(port);
    if (inet_aton(ipAddress.c_str(), &destAddr.sin_addr) == 0)
    {
        return -1;
    }

    int sentBytes = sendto(m_socket, data.c_str(), data.length(), 0, (struct sockaddr*)&destAddr, sizeof(destAddr));

    return sentBytes;
}

int CUdpSocket::receive(std::string &data, std::string &ipAddress, int &port)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    sockaddr_in clientAddr{};
    socklen_t addrLen = sizeof(clientAddr);
    int receivedBytes = recvfrom(m_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &addrLen);

    buffer[receivedBytes] = '\0';
    data = buffer;
    ipAddress = inet_ntoa(clientAddr.sin_addr);
    port = ntohs(clientAddr.sin_port);

    return receivedBytes;
}
