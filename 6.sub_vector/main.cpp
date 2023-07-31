#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <cstdint>
#include <iomanip>

std::string toHexString(uint8_t value)
{
  std::ostringstream oss;
  oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(value);
  return oss.str();
}

std::string vectorToHexString(const std::vector<uint8_t> &data)
{
  std::string hexString;
  for (size_t i = 0; i < data.size(); ++i)
  {
    hexString += toHexString(data[i]);
    if (i < data.size() - 1)
    {
      hexString += " ";
    }
  }
  return hexString;
}

void printRes(const std::vector<uint8_t> &vec)
{
  // std::ostringstream oss;
  // copy(vec.begin(), vec.end(), std::ostream_iterator<int>(oss, " "));
  std::string result = vectorToHexString(vec);
  std::cout << result << std::endl;
  printf("send response is %s\n", result.c_str());
}

int main()
{
  std::vector<uint8_t> oriVec = {0xAB, 0xCD, 0xEF, 0x01, 0x23};
  std::vector<uint8_t> vec;
  uint16_t offset = 2;
  copy(oriVec.begin() + offset, oriVec.end(), std::back_inserter(vec));
  printRes(vec);

  return 0;
}