#include <iostream>

#include "simple_enum.hpp"

enum class Color {
  RED = -100,
  GREEN = 0,
  BLUE = 100,
};

int main() {
  std::cout << simple_enum::enum_name(Color::RED) << std::endl;
  std::cout << simple_enum::enum_name(Color::GREEN) << std::endl;
  std::cout << simple_enum::enum_name(Color::BLUE) << std::endl;
  return 0;
}