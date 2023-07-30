#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[1024];

    // 创建服务器套接字
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(64320); // 使用端口号8080，你可以根据需要更改
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // 绑定套接字
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket." << std::endl;
        return -1;
    }

    // 监听连接
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening." << std::endl;
        return -1;
    }

    std::cout << "Server started. Listening for incoming connections..." << std::endl;

    socklen_t clientAddrSize = sizeof(clientAddr);
    // 接受连接请求
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
    if (clientSocket == -1) {
        std::cerr << "Error accepting connection." << std::endl;
        return -1;
    }

    // 与客户端通信
    while (true) {
        memset(buffer, 0, sizeof(buffer)); // 清空缓冲区

        // 接收数据
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            std::cerr << "Error receiving data." << std::endl;
            break;
        } else if (bytesReceived == 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        }

        std::cout << "Received: " << buffer << std::endl;

        // 回复客户端
        std::string response = "Server received your message.";
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    // 关闭套接字
    close(clientSocket);
    close(serverSocket);

    return 0;
}
