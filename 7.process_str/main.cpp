#include <iostream>
#include <string>
#include <regex> // 包含正则表达式相关头文件
#include <stdio.h>
#include <stdlib.h>

void deleteTargetChar(std::string &str, const char &targetChar) {
    // 使用正则表达式替换所有特定字符为空字符串
    std::regex regexPattern(std::string(1, targetChar));
    str = std::regex_replace(str, regexPattern, "");
}

void testStrtol() {
    char str[] = "0"; // The string representation of the number
    char *endptr; // Used to check for conversion errors
    long int num;

    // Convert the string to a long int
    num = strtol(str, &endptr, 10);

    // Check for conversion errors
    if (!endptr || *endptr != '\0' || errno == ERANGE) {
        printf("Conversion error: %s is not a valid number.\n", str);
    } else {
        printf("The converted number is: %ld\n", num);
    }
}

void testStrtol() {
    char str[] = "0"; // The string representation of the number
    char *endptr; // Used to check for conversion errors
    long int num;

    // Convert the string to a long int
    num = strtol(str, &endptr, 10);

    // Check for conversion errors
    if (!endptr || *endptr != '\0' || errno == ERANGE) {
        printf("Conversion error: %s is not a valid number.\n", str);
    } else {
        printf("The converted number is: %ld\n", num);
    }
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

    testStrtol();

    return 0;
}
