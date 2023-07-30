#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
// #include <cnr/logging.h>
#include <errno.h>
#include <iostream>

const int SERVER_PORT_DMIOT = 64320;
const int MAX_CLIENT_NUM = 5;

int g_iClientNum = 0;
int g_iClientSocketArray[MAX_CLIENT_NUM];

struct protocol
{
    char head = 0;   // 0xc0
    int16_t len = 0; // data size
    char data[];     // Flexible array .need to use malloc() and free()
} __attribute__((packed));

int sendMsg(int fd, const char *msg, size_t len, int flag)
{
    size_t size = sizeof(struct protocol) + len + 1;
    protocol *proto = (struct protocol *)calloc(1, size);
    if (proto)
    {
        proto->head = 0xc0;
        proto->len = len;
        memcpy(proto->data, msg, len);
        // cLOG_INFO << "send proto rpc msg: " << proto->data << ", len = " << size;
        int sendSize = send(fd, proto, size, flag);
        if (sendSize != proto->len + 4)
        {
            // cLOG_ERROR << "Send to miio failed: " << strerror(errno);
        }
        free(proto);
        return sendSize - 4;
    }

    return 0;
}

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
#ifdef CNR
        cLOG_ERROR << "Error creating socket: " << strerror(errno);
#else
        std::cout << "Error creating socket: " << strerror(errno) << std::endl;
#endif
        return -1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT_DMIOT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

//     int optval = 1;
//     if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
//     {
// #ifdef CNR
//         cLOG_ERROR << "setsockopt error: " << strerror(errno);
// #else
//         std::cout << "setsockopt error: " << strerror(errno) << std::endl;
// #endif
//         close(serverSocket);
//         return -1;
//     }

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
#ifdef CNR
        cLOG_ERROR << "Error binding socket: " << strerror(errno);
#else
        std::cout << "Error binding socket: " << strerror(errno) << std::endl;
#endif
        close(serverSocket);
        return -1;
    }

    if (listen(serverSocket, MAX_CLIENT_NUM) < 0)
    {
#ifdef CNR
        cLOG_ERROR << "Error listening: " << strerror(errno);
#else
        std::cout << "Error listening: " << strerror(errno) << std::endl;
#endif
        close(serverSocket);
        return -1;
    }

#ifdef CNR
    cLOG_INFO << "Server started. Listening for incoming connections...";
#else
    std::cout << "Server started. Listening for incoming connections..." << std::endl;
#endif

    socklen_t clientAddrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
    if (clientSocket < 0)
    {
#ifdef CNR
        cLOG_ERROR << "Error accepting connection: " << strerror(errno);
#else
        std::cout << "Error accepting connection: " << strerror(errno) << std::endl;
#endif
        return -1;
    }
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr.s_addr), clientIP, INET_ADDRSTRLEN);

#ifdef CNR
    cLOG_INFO << "Connected to client: ip = " << clientAddr.sin_addr.s_addr << " port: " << ntohs(clientAddr.sin_port);
#else
    std::cout << "Connected to client: ip = " << clientIP << " port: " << ntohs(clientAddr.sin_port) << std::endl;
#endif

    char recvBuf[4096];
    int recvSize = 0;
    while (true)
    {
        memset(recvBuf, 0, sizeof(recvBuf));
        recvSize = recv(clientSocket, recvBuf, sizeof(recvBuf), 0);
        if (recvSize == -1)
        {
#ifdef CNR
            cLOG_ERROR << "Error receiving data";
#else
            std::cout << "Error receiving data " << std::endl;
#endif

            break;
        }
        else if (recvSize == 0)
        {

#ifdef CNR
            cLOG_ERROR << "Client disconnected";
#else
            std::cout << "Client disconnected" << std::endl;
#endif
            break;
        }

#ifdef CNR
        cLOG_INFO << "Received: " << recvBuf << ", length: " << recvSize;
#else
        std::cout << "Received: " << recvBuf << ", length: " << recvSize << std::endl;
#endif

        std::string response = "Server received your message.";
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    close(clientSocket);
    close(serverSocket);

    return 0;
}
