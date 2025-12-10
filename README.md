# CppProjects - C++ 学习项目集合

这是一个包含多个 C++ 学习和实践项目的代码仓库，使用统一的 CMake 构建系统进行管理。

## 项目列表

| 序号 | 项目目录 | 项目名称 | 可执行文件 | 说明 |
|-----|---------|---------|-----------|------|
| 1 | 1.InfoManageSystem | InfoManageSystem | InfoManageSystem | 信息管理系统 |
| 2 | 3.udp_thread | udp_thread | net_thread | UDP 多线程通信 |
| 3 | 5.read_json | testjson | testjson | JSON 读取示例 |
| 4 | 12.md5 | ringbuffer_1 | md5 | MD5 加密（OpenSSL） |
| 5 | 16.thread_pool | thread_pool | thread_pool | 线程池实现 |
| 6 | 18.compress_file | test_zip | test_zip | 文件压缩（Boost + libzip） |
| 7 | 19.check_file | check_file | check_file | 文件检查工具 |
| 8 | 20.blue_ble | blue_ble | blue_ble | 蓝牙 BLE（BlueZ + cnr） |
| 9 | 21.myFsm | myFsm | myFsm | 有限状态机 |
| 10 | 22.test_cnr | test_cnr | test_cnr | CNR 库测试 |
| 11 | 23.simple_enum | simple_enum | simple_enum | 枚举示例 |
| 12 | 24.methodDispatcher | method_dispatcher | method_dispathcer | 方法分发器 |
| 13 | 25.nested_cmake | nested_cmake | test1 | 嵌套 CMake 示例 |
| 14 | 26.sdbus | sdbus | sdbus | D-Bus 客户端（sdbus-c++） |
| 15 | 26.sdbus-bluenet | sdbus-bluenet | ble-server | 蓝牙服务器（sdbus-c++） |

## 环境要求

### 基础依赖
- **CMake**: >= 3.14
- **编译器**: GCC/G++ 或 Clang/Clang++ (支持 C++17)

### 第三方库依赖
不同项目可能需要以下外部库：

| 库名 | 相关项目 | 安装命令（Ubuntu/Debian） |
|-----|---------|--------------------------|
| OpenSSL | 12.md5 | `sudo apt install libssl-dev` |
| Boost | 18.compress_file | `sudo apt install libboost-all-dev` |
| libzip | 18.compress_file | `sudo apt install libzip-dev` |
| sdbus-c++ | 26.sdbus, 26.sdbus-bluenet | 手动编译或查看项目内 lib/ 目录 |
| cnr | 20.blue_ble, 22.test_cnr | 查看项目内 lib/ 目录 |
| pthread | 多个项目 | 系统自带 |

### Spdlog 日志库（Header-Only）
所有项目都配置为可使用 spdlog 进行日志记录。

## 快速开始

### 1. 编译所有项目

```bash
cd /home/dyx/studyspace/CppProjects
cmake -B build
cmake --build build -j$(nproc)
```

所有可执行文件将输出到 `build/bin/` 目录。

### 2. 编译特定项目

#### 编译单个项目

```bash
# 方式 1：使用 PROJECT 变量
cmake -B build -DPROJECT=26.sdbus
cmake --build build

# 方式 2：重新配置
rm -rf build
cmake -B build -DPROJECT=26.sdbus-bluenet
cmake --build build
```

#### 编译多个项目（逗号分隔）

```bash
cmake -B build -DPROJECT="26.sdbus,26.sdbus-bluenet,20.blue_ble"
cmake --build build
```

### 3. 运行程序

```bash
# 查看所有编译好的程序
ls build/bin/

# 运行特定程序
./build/bin/sdbus
./build/bin/blue_ble
./build/bin/test1
```

## 编译选项

### PROJECT 变量

控制编译哪些子项目：

| 值 | 说明 | 示例 |
|---|------|------|
| `all` | 编译所有项目（默认） | `cmake -B build` 或 `cmake -B build -DPROJECT=all` |
| `<项目名>` | 编译单个项目 | `cmake -B build -DPROJECT=26.sdbus` |
| `<项目1>,<项目2>` | 编译多个项目 | `cmake -B build -DPROJECT="26.sdbus,20.blue_ble"` |

### 构建类型

```bash
# Debug 模式（默认）
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Release 模式（优化）
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# 带调试符号的 Release
cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
```

### 清理构建

```bash
# 完全清理
rm -rf build

# 仅清理编译产物（保留 CMake 缓存）
cmake --build build --target clean
```

## 输出目录结构

```
CppProjects/
├── build/                      # 构建目录（统一输出）
│   ├── bin/                   # 所有可执行文件
│   │   ├── sdbus
│   │   ├── ble-server
│   │   ├── blue_ble
│   │   └── ...
│   ├── lib/                   # 所有库文件
│   │   ├── libclac.a         # 来自 25.nested_cmake
│   │   └── ...
│   └── <各子项目构建文件>/
├── third_party/               # 第三方库（共享）
│   └── spdlog/               # Spdlog header-only
│       └── include/
│           └── spdlog/
├── CMakeLists.txt            # 顶层构建文件
└── <各子项目目录>/
```

## 使用 Spdlog 日志

所有项目都已配置好 spdlog 的 include 路径，可以直接使用：

```cpp
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main() {
    // 基本日志
    spdlog::info("Hello, spdlog!");
    spdlog::warn("This is a warning");
    spdlog::error("This is an error");

    // 创建自定义 logger
    auto console = spdlog::stdout_color_mt("console");
    console->info("Welcome to spdlog!");

    // 格式化输出
    spdlog::info("Integer: {}, Float: {:.2f}", 42, 3.14159);

    return 0;
}
```

## 常见问题

### Q1: 编译时提示找不到 spdlog？

**A**: 确保已按照"快速开始"步骤安装 spdlog：
```bash
mkdir -p third_party
cd third_party
git clone --depth 1 https://github.com/gabime/spdlog.git
```

### Q2: 编译某个项目失败，提示缺少库？

**A**: 检查该项目的外部依赖，安装对应的开发库。例如：
```bash
# 12.md5 需要 OpenSSL
sudo apt install libssl-dev

# 18.compress_file 需要 Boost 和 libzip
sudo apt install libboost-all-dev libzip-dev
```

### Q3: 如何只编译某个项目而不影响其他项目？

**A**: 使用 `PROJECT` 变量：
```bash
cmake -B build -DPROJECT=26.sdbus
cmake --build build
```

### Q4: 编译后找不到可执行文件？

**A**: 所有可执行文件都在 `build/bin/` 目录下：
```bash
ls build/bin/
./build/bin/<程序名>
```

### Q5: 如何切换编译器（GCC/Clang）？

**A**: 在配置时指定编译器：
```bash
# 使用 Clang
cmake -B build -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++

# 使用 GCC（默认）
cmake -B build -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
```

### Q6: 为什么有些项目编译失败？

**A**: 可能原因：
1. 缺少外部依赖库（参考"环境要求"安装）
2. 项目本身的代码问题
3. 编译器版本不兼容

可以单独编译成功的项目，跳过有问题的项目。

## 项目结构说明

### 顶层 CMakeLists.txt

顶层 CMakeLists.txt 提供以下功能：

1. **全局配置**: C++17 标准、编译命令导出
2. **Spdlog 配置**: 统一 include 路径，所有子项目可用
3. **统一输出**: 所有可执行文件输出到 `build/bin/`
4. **选择性编译**: 通过 `PROJECT` 变量控制编译范围
5. **编译信息**: 显示构建配置和输出目录

### 子项目兼容性

- 每个子项目仍保持独立的 CMakeLists.txt
- 子项目使用 `CMAKE_CURRENT_SOURCE_DIR` 引用本地路径
- 可以在子项目目录独立编译（部分项目）

## 开发建议

1. **新增项目**: 在对应目录创建 CMakeLists.txt，并将项目名添加到顶层 CMakeLists.txt 的 `ALL_PROJECTS` 列表

2. **使用 spdlog**: 推荐所有新项目使用 spdlog 进行日志记录，便于调试和问题追踪

3. **编译测试**: 修改代码后，使用 `cmake --build build` 增量编译，速度更快

4. **代码导航**: 使用生成的 `compile_commands.json` 配置 IDE/编辑器，获得更好的代码补全和跳转

## 各项目详细说明

### 1.InfoManageSystem
学生信息管理系统

### 2.ring_buffer
c++ 不同方法实现环形缓冲区

### 3.udp_thread
多线程实现 udp socket 通信以及文件 IO

### 4.tcp_com
tcp socket 通信，客户端+服务端

### 5.read_json
使用 rapidjson 解析 json 字符串，存储 json 文件

### 6.process_vector
处理 std::vector
1. vector 转16进制字符串
2. push_back 和 insert 速度对比
3. vector 分包
4. 清空 vector 的方法

### 7.process_str
处理 std::string
1. 使用正则表达式删除所有指定字符
2. 使用正则表达式将字符串中的引号处理为 \"

### 8.process_file
处理文件
1. 使用 std::istreambuf_iterator 配合 copy 算法实现一次性读取整个文本文件
2. 读写文件

### 9.get_wlan_mac
使用 socket 套接字配合 ioctl 函数获取网卡 mac 地址，并转换为 string 输出

### 10.pipe_com
使用 mkfifo 配合 fifo 实现 c++ 程序和 shell 脚本的管道通信

### 11.main_func
main 函数获取输入

### 12.md5
使用 md5 给文件加密

### 13.keyboard
使用 ncurses 读取键盘输入

### 14.process_list
处理 std::list
1. 插入元素
2. 删除元素
3. 找到指定元素删除后再插入

### 16.thread_pool
线程池

### 17.doublyLinkedList
自定义双链表

### 18.compress_file
文件压缩工具

### 19.check_file
文件检查工具

### 20.blue_ble
蓝牙 BLE 通信（基于 BlueZ 和 cnr 库）

### 21.myFsm
有限状态机实现

### 22.test_cnr
CNR 库测试程序

### 23.simple_enum
枚举类型使用示例

### 24.methodDispatcher
方法分发器实现

### 25.nested_cmake
嵌套 CMake 项目示例

### 26.sdbus
D-Bus 客户端（基于 sdbus-c++）

### 26.sdbus-bluenet
蓝牙 BLE 服务器（基于 sdbus-c++）

