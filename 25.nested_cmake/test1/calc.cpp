#include "calc.h"

#include <iostream>

int main() {
  std::cout << "5 + 3 = " << calc::add<int>(5, 3) << std::endl;
  std::cout << "5 -3 = " << calc::sub<int>(5, 3) << std::endl;
  return 0;
}