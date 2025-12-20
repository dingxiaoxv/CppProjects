#include <cstdint>
#include <cstring>
#include <fstream>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include <vector>

struct PoseData {
  uint8_t perfix{0xCE};
  int x : 20;
  int y : 20;
  uint8_t yaw;
  uint8_t tail{0xCE};
} __attribute__((packed));

struct MappingPoseData {
  uint8_t perfix{0xCE};
  int x : 20;
  int y : 20;
  uint8_t yaw;
  uint16_t area;
  uint8_t tail{0xCE};
} __attribute__((packed));

struct MappingErasePoseData {
  uint8_t perfix{0xCE};
  int x : 20;
  int y : 20;
  uint8_t yaw;
  int erase_x : 20;
  int erase_y : 20;
  uint8_t tail{0xCE};
} __attribute__((packed));

struct WorkingPoseData {
  uint8_t perfix{0xCE};
  int x : 20;
  int y : 20;
  uint8_t yaw;
  int startIndex : 24;  // 轨迹点开始索引
  int16_t tracks[3][2]; // 前3个点
  uint8_t region_id;
  uint8_t task_id; // 任务 ID 修改任务id (20250630 由 group_id 修改)
  uint16_t percent;
  int total : 24;
  int finished : 24;
  uint8_t tail{0xCE};
} __attribute__((packed));

struct PoseUnit {
  int x;
  int y;
  int16_t yaw;
};

bool parseLine(const std::string &line, std::vector<uint8_t> &data) {
  data.clear();
  if (line.empty() || line[0] != '[')
    return false;

  std::string content = line.substr(1, line.find(']') - 1);
  std::stringstream ss(content);
  std::string token;

  while (std::getline(ss, token, ',')) {
    data.push_back(static_cast<uint8_t>(std::stoi(token)));
  }
  return !data.empty();
}

void printPoseData(const PoseData &data, int lineNum) {
  spdlog::info("=== PoseData #{} ===", lineNum);
  spdlog::info("perfix: 0x{:x}", (int)data.perfix);
  spdlog::info("x: {}, y: {}, yaw: {}", data.x, data.y, (int)data.yaw);
  spdlog::info("tail: 0x{:x}", (int)data.tail);
  spdlog::info("");
}

void printMappingPoseData(const MappingPoseData &data, int lineNum) {
  spdlog::info("=== MappingPoseData #{} ===", lineNum);
  spdlog::info("perfix: 0x{:x}", (int)data.perfix);
  spdlog::info("x: {}, y: {}, yaw: {}", data.x, data.y, (int)data.yaw);
  spdlog::info("area: {}", data.area);
  spdlog::info("tail: 0x{:x}", (int)data.tail);
  spdlog::info("");
}

void printMappingErasePoseData(const MappingErasePoseData &data, int lineNum) {
  spdlog::info("=== MappingErasePoseData #{} ===", lineNum);
  spdlog::info("perfix: 0x{:x}", (int)data.perfix);
  spdlog::info("x: {}, y: {}, yaw: {}", data.x, data.y, (int)data.yaw);
  spdlog::info("erase_x: {}, erase_y: {}", data.erase_x, data.erase_y);
  spdlog::info("tail: 0x{:x}", (int)data.tail);
  spdlog::info("");
}

void printWorkingPoseData(const WorkingPoseData &data, int lineNum) {
  spdlog::info("=== WorkingPoseData #{} ===", lineNum);
  spdlog::info("perfix: 0x{:x}", (int)data.perfix);
  spdlog::info("x: {}, y: {}, yaw: {}", data.x, data.y, (int)data.yaw);
  spdlog::info("startIndex: {}", data.startIndex);
  spdlog::info("tracks: [{},{}] [{},{}] [{},{}]", data.tracks[0][0], data.tracks[0][1],
               data.tracks[1][0], data.tracks[1][1], data.tracks[2][0], data.tracks[2][1]);
  spdlog::info("region_id: {}, task_id: {}", (int)data.region_id, (int)data.task_id);
  spdlog::info("percent: {}", data.percent);
  spdlog::info("total: {}, finished: {}", data.total, data.finished);
  spdlog::info("tail: 0x{:x}", (int)data.tail);
  spdlog::info("");
}

int main(int argc, char **argv) {
  spdlog::set_default_logger(spdlog::stdout_color_mt("pose"));
  spdlog::set_level(spdlog::level::info);

  if (argc < 2) {
    spdlog::error("用法: {} <数据文件路径>", argv[0]);
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    spdlog::error("无法打开文件: {}", argv[1]);
    return 1;
  }

  std::string line;
  int lineNum = 1;

  while (std::getline(file, line)) {
    std::vector<uint8_t> rawData;
    if (!parseLine(line, rawData)) {
      spdlog::error("第 {} 行解析失败", lineNum);
      lineNum++;
      continue;
    }

    size_t dataSize = rawData.size();

    if (dataSize == sizeof(PoseData)) {
      PoseData data;
      std::memcpy(&data, rawData.data(), sizeof(PoseData));
      printPoseData(data, lineNum);
    } else if (dataSize == sizeof(MappingPoseData)) {
      MappingPoseData data;
      std::memcpy(&data, rawData.data(), sizeof(MappingPoseData));
      printMappingPoseData(data, lineNum);
    } else if (dataSize == sizeof(MappingErasePoseData)) {
      MappingErasePoseData data;
      std::memcpy(&data, rawData.data(), sizeof(MappingErasePoseData));
      printMappingErasePoseData(data, lineNum);
    } else if (dataSize == sizeof(WorkingPoseData)) {
      WorkingPoseData data;
      std::memcpy(&data, rawData.data(), sizeof(WorkingPoseData));
      printWorkingPoseData(data, lineNum);
    } else {
      spdlog::warn("第 {} 行: 未知数据长度 {} 字节", lineNum, dataSize);
    }

    lineNum++;
  }

  file.close();
  return 0;
}
