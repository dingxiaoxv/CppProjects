#include <iostream>
#include <string>
#include <regex> // 包含正则表达式相关头文件

void deleteTargetChar(std::string &str, const char &targetChar) {
    // 使用正则表达式替换所有特定字符为空字符串
    std::regex regexPattern(std::string(1, targetChar));
    str = std::regex_replace(str, regexPattern, "");
}

int main() {
    std::string input = "Hello-world-this-is-a-test";
    char targetChar = '-';

    deleteTargetChar(input, targetChar);

    std::cout << "Modified String: " << input << std::endl;

    return 0;
}
