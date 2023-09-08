#include <fstream>
#include <unistd.h>
#include <iostream>

// const char *MAC_FILE = "/mnt/private/ULI/factory/mac.txt";
const char *MAC_FILE = "mac.txt";

int main() {
    std::string mac = "D4:F0:EA:E8:7B:32";
    std::ofstream ofs(MAC_FILE);
    if (!ofs.is_open()) {
        std::cout << "write file not found" << std::endl;
    }
    ofs << mac;
    ofs.close();

    sleep(3);

    std::ifstream ifs(MAC_FILE);
    if (!ifs.is_open()) {
        std::cout << "read file not found" << std::endl;
    }
    std::string str;
    ifs >> str;
    ifs.close();
    
    std::cout << "mac is " << str << std::endl;

    return 0;
}