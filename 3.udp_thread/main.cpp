#include "cudpsocket.h"
#include "cfileio.h"
#include <iostream>
#include <thread>
#include <deque>
#include <mutex>
#include <condition_variable>

std::deque<std::string> g_dataQueue;
std::mutex g_dataMutex;
std::condition_variable g_dataCondition;

void receiveThreadTask(CUdpSocket &sock)
{
    std::string data, ipAddress;
    int port = 0;

    while (true)
    {
        if (sock.receive(data, ipAddress, port) > 0)
        {
            std::cout << "received from " << ipAddress << "/" << port << " :" << data << std::endl;
            std::lock_guard<std::mutex> lock(g_dataMutex);
            g_dataQueue.push_back(data);
            g_dataCondition.notify_one();
        }
    }
}

void sendThreadTask(CUdpSocket &sock)
{
    while (true)
    {
        std::string data = "Hello, client!";
        std::string ipAddress = "127.0.0.1";
        int port = 1234;
        if (sock.send(data, ipAddress, port) > 0)
        {
            std::cout << "sent to " << ipAddress << "/" << port << " :" << data << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

std::string processData(const std::string &recvData)
{
    return std::string("Processed: " + recvData);
}

int main()
{
    try
    {
        CUdpSocket udpSocket(1234);
        std::thread receiveThread(receiveThreadTask, std::ref(udpSocket));
        std::thread sendThread(sendThreadTask, std::ref(udpSocket));

        receiveThread.detach();
        sendThread.detach();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    CFileIO fileIO;
    if (!fileIO.open("log.txt"))
    {
        std::cerr << "Failed to open log file." << std::endl;
        return 1;
    }

    while (true)
    {
        std::unique_lock<std::mutex> lock(g_dataMutex);
        g_dataCondition.wait(lock, []
                             { return !g_dataQueue.empty(); });

        std::string recvData = g_dataQueue.front();
        g_dataQueue.pop_front();
        lock.unlock();

        std::string processedData = processData(recvData);

        // Write the processed data to the file
        std::thread writeThread([&fileIO, &processedData]()
                                { fileIO.write(processedData); });
        writeThread.join();
    }

    // Close the log file
    fileIO.close();

    return 0;
}
