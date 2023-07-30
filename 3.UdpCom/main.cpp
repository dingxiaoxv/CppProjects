#include "cudpsocket.h"
#include "cfileio.h"
#include "global.h"
#include <iostream>
#include <thread>

std::deque<std::string> g_dataQueue;
std::mutex g_dataMutex;
std::condition_variable g_dataCondition;
CUdpSocket g_udpSocket;

void ReceiveThread()
{
    std::string data, ipAddress;
    int port;

    while (true)
    {
        if (g_udpSocket.Receive(data, ipAddress, port))
        {
            std::lock_guard<std::mutex> lock(g_dataMutex);
            g_dataQueue.push_back(data);
            g_dataCondition.notify_one();
        }
    }
}

void SendThread()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::string data = "Hello, client!";
        std::string ipAddress = "127.0.0.1";
        int port = 1234;

        g_udpSocket.Send(data, ipAddress, port);
    }
}

int main()
{
    CFileIO fileIO;
    // Initialize UDP socket
    if (!g_udpSocket.Initialize(1234))
    {
        std::cerr << "Failed to initialize UDP socket." << std::endl;
        return 1;
    }

    // Open the log file
    if (!fileIO.Open("log.txt"))
    {
        std::cerr << "Failed to open log file." << std::endl;
        return 1;
    }

    // Start the receive thread
    std::thread receiveThread(ReceiveThread);
    // Start the send thread
    std::thread sendThread(SendThread);

    receiveThread.detach();
    sendThread.detach();

    // Process data
    while (true)
    {
        std::unique_lock<std::mutex> lock(g_dataMutex);
        g_dataCondition.wait(lock, []
                            { return !g_dataQueue.empty(); });

        std::string data = g_dataQueue.front();
        g_dataQueue.pop_front();
        lock.unlock();

        // Process the received data here
        std::string processedData = "Processed: " + data;

        // Write the processed data to the file
        std::thread writeThread([&fileIO](std::string processedData)
                               { fileIO.Write(processedData); });
        writeThread.join();
    }

    // Close the log file
    fileIO.Close();

    return 0;
}
