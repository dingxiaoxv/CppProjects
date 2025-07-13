#include <iostream>

#include "simple_enum.hpp"

enum class Color {
  RED,
  GREEN,
  BLUE,
};

int main() {
  std::cout << simple_enum::enum_name<Color::RED>() << std::endl;
  return 0;
}