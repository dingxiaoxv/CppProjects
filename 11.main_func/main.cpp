#include <iostream>

int main(int argc, const char* argv[]) {
    std::cout << "arg num: " << argc << std::endl;
    for (size_t i = 0; i < argc; i++)
    {
        std::cout << "The " << i << " arg is " << argv[i] << std::endl;
    }

    return 0;
}