#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <iostream>
#include <fstream>

void testFunc(const char *origin)
{
    rapidjson::Document doc;
    printf("%s\n", origin);
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
    FILE *jsonFile = fopen(jsonFilePath, "w");
    if (jsonFile != NULL)
    {
        fwrite(buffer.GetString(), sizeof(char), buffer.GetSize(), jsonFile);
        fclose(jsonFile);
        std::cout << "json file saved succeed!" << std::endl;
    }
    else
    {
        std::cout << "json file saved failed!" << std::endl;
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
        text+=line;
    }
    std::cout << text << std::endl;
    unsigned char origin[1024] = text.c_str();
    testFunc(origin);

    return 0;
}