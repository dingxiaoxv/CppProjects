#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string.h>
#include <vector>

std::vector<std::vector<uint8_t>> notificationQueue_;
std::vector<uint8_t> rosRespQueue_;

int64_t getLocalTimeStamp() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

std::string toHexString(uint8_t value) {
  std::ostringstream oss;
  oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(value);
  return oss.str();
}

std::string vectorToHexString_1(const std::vector<uint8_t> &data) {
  std::string hexString;
  for (size_t i = 0; i < data.size(); ++i) {
    hexString += toHexString(data[i]);
    if (i < data.size() - 1) {
      hexString += " ";
    }
  }
  return hexString;
}

std::string vectorToHexString_2(const std::vector<uint8_t> &vec) {
  std::ostringstream oss;
  oss << std::hex << std::setfill('0');
  for (const auto &value : vec) {
    oss << std::setw(2) << static_cast<int>(value);
  }
  return oss.str();
}

int64_t pushTime(const std::vector<uint8_t> &vec) {
  int64_t begin = getLocalTimeStamp();
  notificationQueue_.push_back(vec);
  int64_t end = getLocalTimeStamp();
  std::cout << "pushTime: " << end - begin << std::endl;
  return end - begin;
}

int64_t insertTime(const std::vector<uint8_t> &vec) {
  int64_t begin = getLocalTimeStamp();
  rosRespQueue_.reserve(rosRespQueue_.size() + vec.size());
  rosRespQueue_.insert(rosRespQueue_.end(), vec.begin(), vec.end());
  int64_t end = getLocalTimeStamp();
  std::cout << "insertTime: " << end - begin << std::endl;
  return end - begin;
}

std::vector<std::vector<uint8_t>> subVector(const std::vector<uint8_t> &vec, size_t size) {
  std::vector<std::vector<uint8_t>> packets;
  for (auto it = vec.begin(); it < vec.end(); it += size) {
    if (it + size < vec.end()) {
      packets.push_back(std::vector<uint8_t>(it, it + size));
    } else {
      packets.push_back(std::vector<uint8_t>(it, vec.end()));
    }
  }
}

void testClear(std::vector<uint8_t> &vec) {
  if (vec.empty()) {
    std::cout << "vec is empty" << std::endl;
  }
  // vec.resize(20);
  // memset(&vec[0], 0, 20);
  vec.clear();

  if (vec.empty()) {
    std::cout << "vec is empty" << std::endl;
  }
}

int main() {
  // std::vector<uint8_t> oriVec = {0xAB, 0xCD, 0xEF, 0x01, 0x23};
  // std::cout << vectorToHexString_1(oriVec) << std::endl;
  // std::cout << vectorToHexString_2(oriVec) << std::endl;

  // int64_t pushElapsedTime = 0, insertElapsedTime = 0;
  // for (size_t i = 0; i < 10; i++)
  // {
  //   std::vector<uint8_t> vec(20, i+1);
  //   pushElapsedTime+=pushTime(vec);
  //   insertElapsedTime+=insertTime(vec);
  // }
  // std::cout << "push average time: " << pushElapsedTime/10 << " ms" <<
  // std::endl; std::cout << "insert average time: " << insertElapsedTime/10 <<
  // " ms" << std::endl;

  // std::vector<uint8_t> vecOri, vec1(20, 1), vec2(20, 2), vec3(10, 3);
  // vecOri.reserve(50);
  // vecOri.insert(vecOri.end(), vec1.begin(), vec1.end());
  // vecOri.insert(vecOri.end(), vec2.begin(), vec2.end());
  // vecOri.insert(vecOri.end(), vec3.begin(), vec3.end());
  // std::vector<std::vector<uint8_t>> packets = subVector(vecOri, 20);
  // for (const auto &packet : packets)
  // {
  //   std::cout << vectorToHexString_2(packet) << std::endl;
  // }

  std::vector<uint8_t> vec(20, 1);
  testClear(vec);

  return 0;
}