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

int main() {
    std::string input = "Hello-world-this-is-a-test";
    char targetChar = '-';

    deleteTargetChar(input, targetChar);

    std::cout << "Modified String: " << input << std::endl;

    testStrtol();

    return 0;
}
