#include <iostream>
#include <string>
#include <regex> // 包含正则表达式相关头文件

void deleteTargetChar(std::string &str, const char &targetChar) {
    // 使用正则表达式替换所有特定字符为空字符串
    std::regex regexPattern(std::string(1, targetChar));
    str = std::regex_replace(str, regexPattern, "");
}

void processQuote(const char *str) {
    // 使用正则表达式替换所有反斜杠
    // std::regex backslashPattern("\\\\");
    // std::string backslashReplacement = "\\\\\\\\";
    // std::string backslashResult = std::regex_replace(str, backslashPattern, backslashReplacement);

    // 使用正则表达式替换所有引号
    std::regex quotePattern("\"");
    std::string quoteReplacement = "\\\"";
    std::string result = std::regex_replace(str, quotePattern, quoteReplacement);

    // 输出结果
    std::cout << "Modified string: " << result << std::endl;
}

int main(int argc, const char* argv[]) {

    // std::string input = "test:{This is a "quoted" string with \\ backslashes.}";
    // char targetChar = '-';

    // deleteTargetChar(input, targetChar);

    processQuote(argv[1]);

    // std::cout << "Modified String: " << input << std::endl;

    return 0;
}
