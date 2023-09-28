#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

void timerCallback(void *user_data) {
  std::shared_ptr<int> ptr = *static_cast<std::shared_ptr<int>*>(user_data);
  std::cout << "thread ptr use count: " << ptr.use_count() << std::endl;
  std::thread t([ptr] {
    // std::cout << "定时器触发了回调函数!" << std::endl;
    std::cout << "timer ptr use count: " << ptr.use_count() << std::endl;
    std::cout << "ptr value is " << *ptr << std::endl;
  });
  t.detach();
}

int main() {
  // std::chrono::seconds interval(1); // 定时器间隔为1秒
  // std::shared_ptr<int> ptr = std::make_shared<int>(9);
  // std::cout << "1st ptr use count: " << ptr.use_count() << std::endl;
  // std::thread timerThread([interval, &ptr]() {
  //   while (true) {
  //     std::this_thread::sleep_for(interval);
  //     timerCallback(&ptr);
  //   }
  // });

  // // 主线程休眠一段时间
  // std::this_thread::sleep_for(std::chrono::seconds(5));

  std::shared_ptr<int> ptrB;
  std::cout << "ptrB use count: " << ptrB.use_count() << std::endl;
  std::shared_ptr<int> ptrC = std::make_shared<int>(11);;
  std::cout << "ptrC use count: " << ptrC.use_count() << std::endl;
  {
    std::shared_ptr<int> ptrA = std::make_shared<int>(42);
    std::cout << "ptrA use count: " << ptrA.use_count() << std::endl;
    ptrB = ptrA;
    std::cout << "ptrB value " << *ptrB << std::endl;
    std::cout << "ptrA use count: " << ptrA.use_count() << std::endl;
    std::cout << "ptrB use count: " << ptrB.use_count() << std::endl;
  }
  std::cout << "ptrB value " << *ptrB << std::endl;
  std::cout << "ptrB use count: " << ptrB.use_count() << std::endl;
  ptrB = ptrC;
  std::cout << "ptrB value " << *ptrB << std::endl;
  std::cout << "ptrB use count: " << ptrB.use_count() << std::endl;
  std::cout << "ptrC use count: " << ptrC.use_count() << std::endl;

  // 停止定时器线程
  // timerThread.join();

  return 0;
}
