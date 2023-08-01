#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <iostream>
#include <fstream>

struct DeviceInfo
{
    std::string did;
    std::string key;
    std::string pincode;
};

void testFunc(const char *origin)
{
    rapidjson::Document doc;
    doc.Parse(std::string(origin).c_str());
    if (doc.HasParseError())
    {
        std::cout << "json parse error!" << std::endl;
        return;
    }
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    const char *jsonFilePath = "dmio_pair.json";
    std::ofstream jsonFile(jsonFilePath);
    if (jsonFile.is_open())
    {
        jsonFile << buffer.GetString();
        jsonFile.close();
        std::cout << "json file saved succeed!" << std::endl;
    }
    else
    {
        std::cout << "json file saved failed!" << std::endl;
    }

    DeviceInfo devinfo;
    devinfo.did = "-110010989";
    std::string mac = "C8:5C:CC:A3:28:71";
    std::string model = "dreame.mower.p2255";
    const char *deviceConfName = "device.conf";
    std::ofstream deviceConf(deviceConfName);
    if (deviceConf.is_open())
    {
        deviceConf << "did=" << devinfo.did << "\n";
        deviceConf << "mac=" << mac << "\n";
        deviceConf << "model=" << model;
        deviceConf.close();
        std::cout << "device conf saved succeed!" << std::endl;
    }
    else
    {
        std::cout << "device conf saved failed!" << std::endl;
    }
}

int main()
{
    std::ifstream file("json.txt");
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << std::endl;
        return -1;
    }
    std::string line, text;
    while (getline(file, line))
    {
        text += line;
    }
    testFunc(text.c_str());

    return 0;
}