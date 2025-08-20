#include "calc.h"

namespace calc {

template<>
int sub(const int& lhs, const int& rhs) {
  return lhs - rhs;
}

}