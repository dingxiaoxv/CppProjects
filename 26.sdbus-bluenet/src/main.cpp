#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <thread>

#include "application.hpp"
#include "characteristic.hpp"
#include "service.hpp"

std::atomic<bool> running{true};

void signalHandler(int signal) {
  if (signal == SIGINT || signal == SIGTERM) {
    std::cout << "\nReceived signal " << signal << ", shutting down..."
              << std::endl;
    running = false;
  }
}

int main(int argc, char *argv[]) {
  std::string adapterPath = "/org/bluez/hci0";

  if (argc > 1) {
    adapterPath = argv[1];
  }

  std::cout << "Starting BLE Server..." << std::endl;
  std::cout << "Adapter: " << adapterPath << std::endl;

  // 注册信号处理
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  try {
    // 创建D-Bus连接
    auto connection = sdbus::createSystemBusConnection();
    std::cout << "Connected to system D-Bus" << std::endl;

    // 创建应用
    Application app(*connection, "/com/example/ble");

    // 创建服务 (Heart Rate Service)
    auto heartRateService = std::make_shared<Service>(
        *connection, "/com/example/ble/service0",
        "0000180d-0000-1000-8000-00805f9b34fb", // Heart Rate Service UUID
        true);

    // 创建Heart Rate Measurement特征
    auto hrMeasurement = std::make_shared<Characteristic>(
        *connection, "/com/example/ble/service0/char0",
        "00002a37-0000-1000-8000-00805f9b34fb", // Heart Rate Measurement UUID
        std::vector<std::string>{"read", "notify"},
        "/com/example/ble/service0");

    // 设置读回调
    hrMeasurement->setReadCallback(
        [](const std::map<std::string, sdbus::Variant> &options)
            -> std::vector<uint8_t> {
          std::cout << "Heart Rate read request" << std::endl;
          // 返回格式: [Flags, Heart Rate Value]
          return {0x00, 75}; // 75 bpm
        });

    heartRateService->addCharacteristic(hrMeasurement);

    // 创建自定义数据服务
    auto customService = std::make_shared<Service>(
        *connection, "/com/example/ble/service1",
        "12345678-1234-5678-1234-56789abcdef0", // 自定义UUID
        true);

    // 创建可读写特征
    auto dataCharacteristic = std::make_shared<Characteristic>(
        *connection, "/com/example/ble/service1/char0",
        "12345678-1234-5678-1234-56789abcdef1",
        std::vector<std::string>{"read", "write", "notify"},
        "/com/example/ble/service1");

    // 数据存储
    std::vector<uint8_t> customData = {0x01, 0x02, 0x03, 0x04};

    dataCharacteristic->setReadCallback(
        [&customData](const std::map<std::string, sdbus::Variant> &options)
            -> std::vector<uint8_t> {
          std::cout << "Custom data read request" << std::endl;
          return customData;
        });

    dataCharacteristic->setWriteCallback(
        [&customData](const std::vector<uint8_t> &value,
                      const std::map<std::string, sdbus::Variant> &options) {
          std::cout << "Custom data write request, size: " << value.size()
                    << std::endl;
          customData = value;
        });

    customService->addCharacteristic(dataCharacteristic);

    // 添加服务到应用
    app.addService(heartRateService);
    app.addService(customService);

    // 注册应用到BlueZ
    app.registerToBlueZ(adapterPath);

    std::cout << "\n=== BLE Server Started ===" << std::endl;
    std::cout << "Services:" << std::endl;
    std::cout << "  - Heart Rate Service (UUID: 0000180d-...)" << std::endl;
    std::cout << "  - Custom Data Service (UUID: 12345678-...)" << std::endl;
    std::cout << "\nPress Ctrl+C to stop\n" << std::endl;

    // 启动通知发送线程（模拟心率变化）
    std::thread notifyThread([&hrMeasurement, &dataCharacteristic]() {
      uint8_t heartRate = 70;
      uint8_t counter = 0;

      while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        if (!running)
          break;

        // 模拟心率变化
        heartRate = 70 + (counter % 20);
        std::vector<uint8_t> hrValue = {0x00, heartRate};

        std::cout << "Sending heart rate notification: " << (int)heartRate
                  << " bpm" << std::endl;
        hrMeasurement->notify(hrValue);

        // 发送自定义数据通知
        std::vector<uint8_t> customNotify = {0x06, counter};
        std::cout << "Sending custom notification: counter = " << (int)counter
                  << std::endl;
        dataCharacteristic->notify(customNotify);

        counter++;
      }
    });

    // 进入事件循环
    while (running) {
      connection->processPendingEvent();
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // 等待通知线程结束
    if (notifyThread.joinable()) {
      notifyThread.join();
    }

    std::cout << "BLE Server stopped" << std::endl;

  } catch (const sdbus::Error &e) {
    std::cerr << "D-Bus error: " << e.what() << std::endl;
    return 1;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
