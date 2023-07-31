#include <iostream>
#include <string>
#include <fstream>

const std::string DID_CONF_FILE = "did.txt";
const std::string KEY_CONF_FILE = "key.txt";
const std::string PIN_CODE_FILE = "pincode.txt";

struct DeviceInfo
{
    std::string did;
    std::string key;
    std::string pincode;
};

std::string readWholeFile(const std::string &fileName)
{
    std::ifstream in(fileName);
    if (!in.is_open())
    {
        throw std::runtime_error(std::string("Failed to open the file: ") + fileName);
    }

    std::string res;
    std::copy(
        std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>(),
        std::back_inserter(res));

    return res;
}

DeviceInfo getDeviceInfo()
{
    DeviceInfo info;
    info.did = readWholeFile(DID_CONF_FILE);
    info.key = readWholeFile(KEY_CONF_FILE);
    info.pincode = readWholeFile(PIN_CODE_FILE);
    return info;
}

int main()
{
    // 程序正常退出
    // try
    // {
    //     DeviceInfo info = getDeviceInfo();
    //     std::cout << info.did << std::endl;
    //     std::cout << info.key << std::endl;
    //     std::cout << info.pincode << std::endl;
    // }
    // catch (const std::exception &e)
    // {
    //     std::cerr << "Error: " << e.what() << std::endl;
    //     return -1;
    // }

    // 程序不能正常退出
    DeviceInfo info = getDeviceInfo();
    std::cout << info.did << std::endl;
    std::cout << info.key << std::endl;
    std::cout << info.pincode << std::endl;

    return 0;
}