#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>

void printReq(std::vector<uint8_t> vec)
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (const auto &value : vec)
    {
        oss << std::setw(2) << static_cast<int>(value);
    }
    const char *str = oss.str().data();
    printf("Recv request: %s\n", str);
}

int main(int argc, const char* argv[])
{
    std::vector<uint8_t> vec = {0xAB, 0xCD, 0xEF, 0x01, 0x23};
    printReq(vec);

    return 0;
}