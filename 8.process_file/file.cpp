#include <fstream>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <iomanip>

// const char *MAC_FILE = "/mnt/private/ULI/factory/mac.txt";
const char *MAC_FILE = "mac.txt";
const char *MAP_FILE = "mapConfig.txt";

void writeMap() {
    std::ofstream ofs(MAP_FILE);
    if (!ofs.is_open()) {
        std::cout << "file doesn't exist" << std::endl;;
        return;
    }
    std::vector<uint8_t> vec;
    for (size_t i = 0; i < 1024; i++)
    {
        vec.push_back(i+1);
    }
    ofs.write(reinterpret_cast<const char*>(vec.data()), vec.size());
    ofs.close();
}

void readMap() {
    std::ifstream ifs(MAP_FILE);
    if (!ifs.is_open()) {
        std::cout << "file doesn't exist" << std::endl;;
        return;
    }
    std::vector<uint8_t> vec;
    char byte;
    while (ifs.get(byte))
    {
        vec.push_back(static_cast<uint8_t>(byte));
    }
    ifs.close();
    // 打印数据的十六进制表示
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(vec[i]);
        if ((i + 1) % 16 == 0) {
            std::cout << std::endl; // 每行打印16个字节
        } else {
            std::cout << ' ';
        }
    }
}

int main() {
    // std::string mac = "D4:F0:EA:E8:7B:32";
    // std::ofstream ofs(MAC_FILE);
    // if (!ofs.is_open()) {
    //     std::cout << "write file not found" << std::endl;
    // }
    // ofs << mac;
    // ofs.close();

    // sleep(3);

    // std::ifstream ifs(MAC_FILE);
    // if (!ifs.is_open()) {
    //     std::cout << "read file not found" << std::endl;
    // }
    // std::string str;
    // ifs >> str;
    // ifs.close();
    
    // std::cout << "mac is " << str << std::endl;
    writeMap();
    sleep(1);
    readMap();

    return 0;
}