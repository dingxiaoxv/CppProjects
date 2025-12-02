# sdbus-bluenet - BLE Server using sdbus-c++

一个基于sdbus-c++实现的简单BLE GATT服务器。

## 功能特性

- 使用sdbus-c++库与BlueZ通信
- 实现标准的BlueZ GATT D-Bus API
- 支持Notification通知机制
- 包含两个示例服务：
  - Heart Rate Service (标准蓝牙服务)
  - Custom Data Service (自定义服务)

## 依赖

### 系统依赖
```bash
# Ubuntu/Debian
sudo apt-get install -y \
    build-essential \
    cmake \
    pkg-config \
    libsdbus-c++-dev \
    bluez

# 如果libsdbus-c++-dev不可用，需要从源码编译
git clone https://github.com/Kistler-Group/sdbus-cpp.git
cd sdbus-cpp
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
sudo ldconfig
```

## 编译

```bash
cd /home/dyx/studyspace/sdbus-bluenet
mkdir build
cd build
cmake ..
make
```

## 运行

```bash
# 确保bluetoothd正在运行
sudo systemctl status bluetooth

# 运行BLE服务器（需要root权限访问D-Bus系统总线）
sudo ./ble-server

# 或指定特定的蓝牙适配器
sudo ./ble-server /org/bluez/hci1
```

## 测试

### 使用bluetoothctl测试

```bash
# 在另一个终端
bluetoothctl

# 在bluetoothctl中
[bluetooth]# power on
[bluetooth]# advertise on
[bluetooth]# show
```

### 使用手机测试

1. 在手机上安装BLE扫描工具（如nRF Connect）
2. 扫描附近的BLE设备
3. 连接到您的设备
4. 查看服务和特征
5. 启用通知，观察心率和自定义数据的更新

### 使用gatttool测试

```bash
# 扫描设备
sudo hcitool lescan

# 连接并读取
gatttool -b <MAC_ADDRESS> --characteristics
gatttool -b <MAC_ADDRESS> --char-read -u 00002a37-0000-1000-8000-00805f9b34fb
```

## 项目结构

```
sdbus-bluenet/
├── CMakeLists.txt          # CMake构建配置
├── README.md              # 本文件
├── include/               # 头文件
│   ├── application.hpp    # 应用管理
│   ├── service.hpp        # GATT服务
│   └── characteristic.hpp # GATT特征
└── src/                   # 源文件
    ├── application.cpp
    ├── service.cpp
    ├── characteristic.cpp
    └── main.cpp           # 主程序
```

## 服务说明

### Heart Rate Service (UUID: 0000180d-...)
- **Heart Rate Measurement** (UUID: 00002a37-...)
  - 属性: read, notify
  - 功能: 读取心率值，每2秒推送通知

### Custom Data Service (UUID: 12345678-...)
- **Data Characteristic** (UUID: 12345678-...1)
  - 属性: read, write, notify
  - 功能: 读写自定义数据，每2秒推送计数器通知

## 扩展开发

### 添加新服务

```cpp
// 在main.cpp中添加
auto newService = std::make_shared<Service>(
    *connection,
    "/com/example/ble/service2",
    "your-service-uuid",
    true  // primary
);

auto newCharacteristic = std::make_shared<Characteristic>(
    *connection,
    "/com/example/ble/service2/char0",
    "your-characteristic-uuid",
    std::vector<std::string>{"read", "write", "notify"},
    "/com/example/ble/service2"
);

newService->addCharacteristic(newCharacteristic);
app.addService(newService);
```

### 自定义回调

```cpp
characteristic->setReadCallback([](auto& options) {
    // 处理读取请求
    return std::vector<uint8_t>{0x01, 0x02};
});

characteristic->setWriteCallback([](auto& value, auto& options) {
    // 处理写入请求
    std::cout << "Received: " << value.size() << " bytes" << std::endl;
});
```

## 调试

### 启用BlueZ调试日志
```bash
# 停止bluetooth服务
sudo systemctl stop bluetooth

# 手动启动bluetoothd（带调试）
sudo /usr/libexec/bluetooth/bluetoothd -n -d
```

### 监控D-Bus消息
```bash
dbus-monitor --system "interface='org.bluez.GattCharacteristic1'"
```

### 监控HCI流量
```bash
sudo btmon
```

## 许可证

GPL-2.0

## 参考资料

- [BlueZ GATT API](https://git.kernel.org/pub/scm/bluetooth/bluez.git/tree/doc/gatt-api.txt)
- [sdbus-c++ Documentation](https://github.com/Kistler-Group/sdbus-cpp)
- [Bluetooth GATT Specifications](https://www.bluetooth.com/specifications/gatt/)
