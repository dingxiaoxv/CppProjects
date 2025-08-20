#include "calc.h"

namespace calc {

template<>
int add(const int& lhs, const int& rhs) {
  return lhs + rhs;
}

}