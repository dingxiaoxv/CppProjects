#include "cudpsocket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

CUdpSocket::CUdpSocket()
{
    // Constructor
}

CUdpSocket::~CUdpSocket()
{
    // Destructor
}

bool CUdpSocket::Initialize(int port)
{
    // Initialize UDP socket
    // Create socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        return false;
    }

    // Bind to the specified port
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    if (bind(sock, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) < 0)
    {
        close(sock);
        return false;
    }

    return true;
}

bool CUdpSocket::Send(const std::string &data, const std::string &ipAddress, int port)
{
    // Send UDP data
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        return false;
    }

    sockaddr_in destAddr{};
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(port);
    if (inet_aton(ipAddress.c_str(), &destAddr.sin_addr) == 0)
    {
        close(sock);
        return false;
    }

    ssize_t sentBytes = sendto(sock, data.c_str(), data.length(), 0, reinterpret_cast<sockaddr *>(&destAddr), sizeof(destAddr));
    close(sock);

    return (sentBytes == static_cast<ssize_t>(data.length()));
}

bool CUdpSocket::Receive(std::string &data, std::string &ipAddress, int &port)
{
    // Receive UDP data
    char buffer[1024];
    sockaddr_in clientAddr{};
    socklen_t addrLen = sizeof(clientAddr);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        return false;
    }

    ssize_t receivedBytes = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, reinterpret_cast<sockaddr *>(&clientAddr), &addrLen);
    close(sock);

    if (receivedBytes <= 0)
    {
        return false;
    }

    buffer[receivedBytes] = '\0';
    data = buffer;
    ipAddress = inet_ntoa(clientAddr.sin_addr);
    port = ntohs(clientAddr.sin_port);

    return true;
}
