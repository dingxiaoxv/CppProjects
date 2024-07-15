#pragma once

#include <functional>

namespace cnr {

namespace utils {
  void setRealtimePriority(int priority);
  void runInThread(std::function<void()> func);
}

}