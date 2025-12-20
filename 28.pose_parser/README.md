# Pose Parser

一个用于解析机器人位姿数据的C++工具，支持解析多种不同格式的位姿数据结构。

## 功能特性

- 支持解析4种不同的位姿数据格式：
  - **PoseData**: 基本位姿数据（x, y, yaw）
  - **MappingPoseData**: 建图位姿数据（包含区域信息）
  - **MappingErasePoseData**: 建图擦除位姿数据（包含擦除坐标）
  - **WorkingPoseData**: 工作位姿数据（包含轨迹、任务、进度等详细信息）
- 自动识别数据类型并进行解析
- 使用spdlog进行格式化日志输出
- 支持位域(bit field)压缩的数据结构

## 数据结构说明

### PoseData (6字节)
```cpp
struct PoseData {
  uint8_t perfix;    // 前缀 0xCE
  int x : 20;        // x坐标 (20位)
  int y : 20;        // y坐标 (20位)
  uint8_t yaw;       // 偏航角
  uint8_t tail;      // 尾标 0xCE
} __attribute__((packed));
```

### MappingPoseData (8字节)
```cpp
struct MappingPoseData {
  uint8_t perfix;    // 前缀 0xCE
  int x : 20;        // x坐标
  int y : 20;        // y坐标
  uint8_t yaw;       // 偏航角
  uint16_t area;     // 区域ID
  uint8_t tail;      // 尾标 0xCE
} __attribute__((packed));
```

### MappingErasePoseData (11字节)
```cpp
struct MappingErasePoseData {
  uint8_t perfix;    // 前缀 0xCE
  int x : 20;        // 当前x坐标
  int y : 20;        // 当前y坐标
  uint8_t yaw;       // 偏航角
  int erase_x : 20;  // 擦除点x坐标
  int erase_y : 20;  // 擦除点y坐标
  uint8_t tail;      // 尾标 0xCE
} __attribute__((packed));
```

### WorkingPoseData (25字节)
```cpp
struct WorkingPoseData {
  uint8_t perfix;         // 前缀 0xCE
  int x : 20;             // x坐标
  int y : 20;             // y坐标
  uint8_t yaw;            // 偏航角
  int startIndex : 24;    // 轨迹点开始索引
  int16_t tracks[3][2];   // 前3个轨迹点
  uint8_t region_id;      // 区域ID
  uint8_t task_id;        // 任务ID
  uint16_t percent;       // 完成百分比
  int total : 24;         // 总任务数
  int finished : 24;      // 已完成任务数
  uint8_t tail;           // 尾标 0xCE
} __attribute__((packed));
```

## 编译

项目使用CMake构建系统，要求CMake 3.16及以上版本。

```bash
cd 28.pose_parser
mkdir build && cd build
cmake ..
make
```

## 依赖

- C++17或更高版本
- spdlog库（用于日志输出）
- CMake 3.16+

## 使用方法

```bash
./pose_parser <数据文件路径>
```

### 数据文件格式

数据文件应为文本格式，每行包含一个字节数组，格式如下：
```
[206,1,2,3,4,5,206]
[206,10,20,30,40,50,60,206]
```

每行以方括号包围，字节值以逗号分隔。

### 输出示例

程序会自动识别数据类型并输出解析结果：

```
[info] === PoseData #1 ===
[info] perfix: 0xce
[info] x: 1000, y: 2000, yaw: 90
[info] tail: 0xce

[info] === WorkingPoseData #2 ===
[info] perfix: 0xce
[info] x: 1500, y: 2500, yaw: 45
[info] startIndex: 100
[info] tracks: [10,20] [30,40] [50,60]
[info] region_id: 1, task_id: 5
[info] percent: 75
[info] total: 1000, finished: 750
[info] tail: 0xce
```

## 特性说明

- **自动类型识别**: 根据数据长度自动判断数据类型
- **位域支持**: 支持解析使用位域压缩的数据结构
- **错误处理**: 对无法识别的数据长度会给出警告信息
- **彩色日志**: 使用spdlog的彩色输出，提高可读性

## 注意事项

- 所有数据结构使用`__attribute__((packed))`确保内存对齐，防止结构体填充
- 前缀和尾标都使用0xCE作为标识符
- 使用位域时要注意字节序和位序的影响
- 坐标使用20位整数，最大值范围为 -524288 ~ 524287
