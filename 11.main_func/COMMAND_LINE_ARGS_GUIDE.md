# 命令行参数解析详细指南

## 目录
1. [getopt_long 基础](#getopt_long-基础)
2. [选项定义](#选项定义)
3. [参数解析流程](#参数解析流程)
4. [完整示例](#完整示例)
5. [最佳实践](#最佳实践)
6. [常见问题](#常见问题)

---

## getopt_long 基础

`getopt_long` 是 POSIX 标准库函数，用于解析命令行参数。它支持：
- **短选项**：单字符选项，如 `-n` `-a` `-e`
- **长选项**：完整单词选项，如 `--name` `--age` `--email`

### 函数原型
```cpp
int getopt_long(int argc, char *const argv[],
                const char *optstring,
                const struct option *longopts,
                int *longindex);
```

### 参数说明
- `argc`, `argv`: main函数的参数
- `optstring`: 短选项字符串
- `longopts`: 长选项数组
- `longindex`: 返回匹配的长选项索引（可选）

---

## 选项定义

### 1. 短选项字符串 (optstring)

格式规则：
```cpp
const char *short_options = "n:a:e:h";
```

- `n:` - 字符后有冒号，表示该选项**必需**参数
- `h`  - 字符后无冒号，表示该选项**不需要**参数
- `n::` - 两个冒号表示该选项参数是**可选的**（不常用）

#### 示例
```cpp
"abc"     // -a -b -c 都不需要参数
"a:b:c"   // -a -b 需要参数，-c 不需要
"a::bc:"  // -a 参数可选，-b 无参数，-c 需要参数
```

### 2. 长选项数组 (longopts)

使用 `struct option` 定义：
```cpp
struct option {
    const char *name;    // 长选项名称
    int has_arg;        // 参数要求
    int *flag;          // 标志位（通常为nullptr）
    int val;            // 返回值（通常是短选项字符）
};
```

#### has_arg 的三种值：
- `no_argument` (0): 不需要参数
- `required_argument` (1): 必需参数
- `optional_argument` (2): 可选参数

#### 示例
```cpp
const struct option long_options[] = {
    {"name",    required_argument, nullptr, 'n'},  // --name VALUE
    {"age",     required_argument, nullptr, 'a'},  // --age VALUE
    {"email",   required_argument, nullptr, 'e'},  // --email VALUE
    {"help",    no_argument,       nullptr, 'h'},  // --help
    {"verbose", no_argument,       nullptr, 'v'},  // --verbose
    {nullptr,   0,                 nullptr, 0}     // 结束标记（必须）
};
```

---

## 参数解析流程

### 基本解析循环
```cpp
int opt;
while ((opt = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
    switch (opt) {
    case 'n':
        // 处理 -n 或 --name
        name = optarg;  // optarg 包含参数值
        break;
    case 'h':
        // 处理 -h 或 --help
        print_usage();
        break;
    case '?':
        // 未知选项或缺少必需参数
        return 1;
    default:
        // 错误处理
        return 1;
    }
}
```

### 重要全局变量
- `optarg`: 指向当前选项的参数字符串
- `optind`: 下一个要处理的 argv 元素索引
- `opterr`: 是否打印错误信息（默认1）
- `optopt`: 导致错误的选项字符

---

## 完整示例

### 示例1：用户管理客户端

```cpp
#include <getopt.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    // 变量定义
    std::string name;
    uint32_t age = 0;
    std::string email;
    bool verbose = false;

    // 定义选项
    const char *short_options = "n:a:e:vh";
    const struct option long_options[] = {
        {"name",    required_argument, nullptr, 'n'},
        {"age",     required_argument, nullptr, 'a'},
        {"email",   required_argument, nullptr, 'e'},
        {"verbose", no_argument,       nullptr, 'v'},
        {"help",    no_argument,       nullptr, 'h'},
        {nullptr,   0,                 nullptr, 0}
    };

    // 解析参数
    int opt;
    while ((opt = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
        switch (opt) {
        case 'n':
            name = optarg;
            break;
        case 'a':
            try {
                age = std::stoul(optarg);
            } catch (const std::exception &e) {
                std::cerr << "Invalid age: " << optarg << std::endl;
                return 1;
            }
            break;
        case 'e':
            email = optarg;
            break;
        case 'v':
            verbose = true;
            break;
        case 'h':
            std::cout << "Usage: " << argv[0] << " [OPTIONS]\n"
                      << "Options:\n"
                      << "  -n, --name NAME      User name\n"
                      << "  -a, --age AGE        User age\n"
                      << "  -e, --email EMAIL    User email\n"
                      << "  -v, --verbose        Enable verbose output\n"
                      << "  -h, --help           Show this help\n";
            return 0;
        case '?':
            // getopt_long 已打印错误信息
            return 1;
        default:
            std::cerr << "Use --help for usage information\n";
            return 1;
        }
    }

    // 验证必需参数
    if (name.empty() || age == 0 || email.empty()) {
        std::cerr << "Error: --name, --age, and --email are required\n";
        return 1;
    }

    // 处理剩余的非选项参数
    for (int i = optind; i < argc; i++) {
        std::cout << "Non-option argument: " << argv[i] << std::endl;
    }

    // 执行主要逻辑
    if (verbose) {
        std::cout << "Name: " << name << "\n"
                  << "Age: " << age << "\n"
                  << "Email: " << email << std::endl;
    }

    return 0;
}
```

### 使用示例
```bash
# 使用长选项
./program --name "John Doe" --age 25 --email "john@example.com"

# 使用短选项
./program -n "John Doe" -a 25 -e "john@example.com"

# 混合使用
./program --name "John Doe" -a 25 --email "john@example.com" --verbose

# 带引号的参数（包含空格）
./program --name "John Smith" --age 30 --email "john.smith@example.com"

# 查看帮助
./program --help
./program -h
```

---

## 示例2：文件处理工具

```cpp
#include <getopt.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    std::string input_file;
    std::string output_file;
    std::string format = "json";  // 默认值
    int compression_level = 5;     // 默认值
    bool overwrite = false;

    const char *short_options = "i:o:f:c:Oh";
    const struct option long_options[] = {
        {"input",       required_argument, nullptr, 'i'},
        {"output",      required_argument, nullptr, 'o'},
        {"format",      required_argument, nullptr, 'f'},
        {"compression", required_argument, nullptr, 'c'},
        {"overwrite",   no_argument,       nullptr, 'O'},
        {"help",        no_argument,       nullptr, 'h'},
        {nullptr,       0,                 nullptr, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
        switch (opt) {
        case 'i':
            input_file = optarg;
            break;
        case 'o':
            output_file = optarg;
            break;
        case 'f':
            format = optarg;
            if (format != "json" && format != "xml" && format != "csv") {
                std::cerr << "Invalid format: " << format
                          << " (must be json, xml, or csv)\n";
                return 1;
            }
            break;
        case 'c':
            try {
                compression_level = std::stoi(optarg);
                if (compression_level < 0 || compression_level > 9) {
                    throw std::out_of_range("out of range");
                }
            } catch (const std::exception &e) {
                std::cerr << "Invalid compression level: " << optarg
                          << " (must be 0-9)\n";
                return 1;
            }
            break;
        case 'O':
            overwrite = true;
            break;
        case 'h':
            std::cout << "Usage: " << argv[0] << " [OPTIONS]\n"
                      << "Options:\n"
                      << "  -i, --input FILE         Input file\n"
                      << "  -o, --output FILE        Output file\n"
                      << "  -f, --format FORMAT      Output format (json|xml|csv) [default: json]\n"
                      << "  -c, --compression LEVEL  Compression level (0-9) [default: 5]\n"
                      << "  -O, --overwrite          Overwrite existing files\n"
                      << "  -h, --help               Show this help\n";
            return 0;
        default:
            return 1;
        }
    }

    if (input_file.empty()) {
        std::cerr << "Error: --input is required\n";
        return 1;
    }

    std::cout << "Processing:\n"
              << "  Input: " << input_file << "\n"
              << "  Output: " << (output_file.empty() ? "stdout" : output_file) << "\n"
              << "  Format: " << format << "\n"
              << "  Compression: " << compression_level << "\n"
              << "  Overwrite: " << (overwrite ? "yes" : "no") << std::endl;

    return 0;
}
```

---

## 最佳实践

### 1. 参数验证
```cpp
// 必需参数检查
if (name.empty() || age == 0 || email.empty()) {
    std::cerr << "Error: Missing required arguments\n";
    std::cerr << "Use --help for usage information\n";
    return 1;
}

// 数值范围验证
if (age < 1 || age > 150) {
    std::cerr << "Error: Age must be between 1 and 150\n";
    return 1;
}

// 格式验证（正则表达式）
std::regex email_regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
if (!std::regex_match(email, email_regex)) {
    std::cerr << "Error: Invalid email format\n";
    return 1;
}
```

### 2. 提供有用的帮助信息
```cpp
void print_usage(const char *program_name) {
    std::cout << "Usage: " << program_name << " [OPTIONS]\n\n"
              << "Description:\n"
              << "  Add a new user to the database\n\n"
              << "Required Options:\n"
              << "  -n, --name NAME      User's full name\n"
              << "  -a, --age AGE        User's age (1-150)\n"
              << "  -e, --email EMAIL    User's email address\n\n"
              << "Optional:\n"
              << "  -v, --verbose        Enable verbose output\n"
              << "  -h, --help           Show this help message\n\n"
              << "Examples:\n"
              << "  " << program_name << " --name \"John Doe\" --age 25 --email john@example.com\n"
              << "  " << program_name << " -n \"Jane Smith\" -a 30 -e jane@example.com -v\n";
}
```

### 3. 错误处理
```cpp
// 禁用 getopt 的自动错误消息（可选）
opterr = 0;

while ((opt = getopt_long(...)) != -1) {
    switch (opt) {
    // ... 正常处理 ...
    case '?':
        if (optopt == 'n') {
            std::cerr << "Option -n requires an argument\n";
        } else {
            std::cerr << "Unknown option: " << (char)optopt << "\n";
        }
        return 1;
    }
}
```

### 4. 默认值设置
```cpp
// 在解析前设置默认值
std::string format = "json";
int timeout = 30;
bool verbose = false;

// 解析后保留默认值（如果未提供）
```

### 5. 处理非选项参数
```cpp
// 所有选项解析完成后，optind 指向第一个非选项参数
for (int i = optind; i < argc; i++) {
    std::cout << "Processing file: " << argv[i] << std::endl;
}

// 示例: program --name John file1.txt file2.txt
// optind 将指向 "file1.txt"
```

---

## 常见问题

### Q1: 如何支持可选参数？
```cpp
const char *short_options = "o::";  // 两个冒号
const struct option long_options[] = {
    {"output", optional_argument, nullptr, 'o'},
    {nullptr, 0, nullptr, 0}
};

// 使用
case 'o':
    if (optarg) {
        output_file = optarg;
    } else {
        output_file = "default_output.txt";
    }
    break;
```

### Q2: 如何处理多次出现的选项？
```cpp
std::vector<std::string> input_files;

case 'i':
    input_files.push_back(optarg);
    break;

// 使用: program -i file1.txt -i file2.txt -i file3.txt
```

### Q3: 如何处理布尔标志？
```cpp
bool verbose = false;
bool debug = false;
bool quiet = false;

case 'v': verbose = true; break;
case 'd': debug = true; break;
case 'q': quiet = true; break;

// 使用: program -v -d    或    program -vd
```

### Q4: 如何重置 getopt 状态？
```cpp
// 在某些情况下需要重新解析
optind = 1;  // Linux/BSD
optreset = 1;  // BSD only
```

### Q5: 参数中包含空格或特殊字符？
```bash
# Shell 会自动处理引号，程序接收的是完整字符串
./program --name "John Doe"
./program --path "/home/user/my folder"
./program --text "Line 1\nLine 2"
```

### Q6: 如何支持 --option=value 格式？
getopt_long 自动支持：
```bash
# 以下三种方式等价
./program --name "John"
./program --name="John"
./program --name=John
```

---

## 进阶技巧

### 1. 长选项专用（无短选项对应）
```cpp
const struct option long_options[] = {
    {"config-file", required_argument, nullptr, 1000},  // 使用大数值
    {"log-level",   required_argument, nullptr, 1001},
    {nullptr, 0, nullptr, 0}
};

switch (opt) {
case 1000:
    config_file = optarg;
    break;
case 1001:
    log_level = optarg;
    break;
}
```

### 2. 使用标志位
```cpp
int verbose_flag = 0;

const struct option long_options[] = {
    {"verbose", no_argument, &verbose_flag, 1},
    {nullptr, 0, nullptr, 0}
};

// getopt_long 会自动设置 verbose_flag = 1
```

### 3. 组合短选项
```bash
# 这些是等价的
./program -v -d -q
./program -vdq
```

---

## 总结

getopt_long 是一个强大且标准的命令行参数解析工具。关键点：

1. **定义清晰的选项**：短选项简洁，长选项可读
2. **完善的错误处理**：验证参数类型和范围
3. **友好的帮助信息**：包含示例和默认值
4. **遵循 Unix 约定**：`-h` 帮助，`-v` 版本/详细，`-q` 安静模式

---

## 参考资源

- `man 3 getopt_long` - 完整的 man page
- [GNU getopt 文档](https://www.gnu.org/software/libc/manual/html_node/Getopt.html)
- POSIX 标准文档
