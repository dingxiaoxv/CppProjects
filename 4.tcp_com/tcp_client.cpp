#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // 创建客户端套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(64320); // 使用服务器端口号，确保与服务器端一致
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // 设置服务器IP地址，这里使用本地回环地址

    // 连接服务器
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server." << std::endl;
        return -1;
    }

    // 与服务器通信
    while (true) {
        std::string message;
        std::cout << "Enter message (type 'exit' to quit): ";
        std::getline(std::cin, message);

        if (message == "exit")
            break;

        // 发送数据
        send(clientSocket, message.c_str(), message.size(), 0);

        memset(buffer, 0, sizeof(buffer)); // 清空缓冲区

        // 接收服务器响应
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            std::cerr << "Error receiving data." << std::endl;
            break;
        } else if (bytesReceived == 0) {
            std::cout << "Server disconnected." << std::endl;
            break;
        }

        std::cout << "Server response: " << buffer << std::endl;
    }

    // 关闭套接字
    close(clientSocket);

    return 0;
}
