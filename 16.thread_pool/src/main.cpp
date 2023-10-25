#include "threadPool.h"
#include <iostream>

// void callback(ThreadPool& threadPool) {
//     std::cout << "Callback is running." << std::endl;

//     // 在回调函数中将任务提交到线程池
//     threadPool.enqueue([]() {
//         // 执行后台任务
//         std::this_thread::sleep_for(std::chrono::seconds(2));
//         std::cout << "Background task completed." << std::endl;
//     });

//     std::cout << "Callback is done." << std::endl;
// }

void callback() {
    std::cout << "Callback is running." << std::endl;

    // 在回调函数中将任务提交到线程池
    std::thread t([] {
        // 执行后台任务
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "Background task completed." << std::endl;
    });
    t.detach();

    std::cout << "Callback is done." << std::endl;
}

int main() {
    // 创建线程池
    // ThreadPool threadPool(4); // 4个线程的线程池

    // 调用回调函数
    // callback(threadPool);
    callback();

    size_t cnt = 0;
    while (cnt < 30)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 等待一段时间，确保后台任务有机会完成
      std::cout << "main thread " << cnt << std::endl;
      ++cnt;
    }
    
    std::cout << "Main thread exiting." << std::endl;
    return 0;
}
