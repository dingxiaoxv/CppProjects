#include <iostream>
#include <string>

#include "method_dispatcher.hpp"

struct Request {
  std::string data;
  int value;
};

int main() {
  std::cout << "=== MethodDispatcher Examples ===" << std::endl;

  // Example 1: void return type (backward compatible)
  std::cout << "\n1. Void return type:" << std::endl;
  MethodDispatcher<Request> voidDispatcher;
  
  voidDispatcher.registerHandler("print", [](const Request& req) {
    std::cout << "  Handling print: " << req.data << std::endl;
  });
  
  voidDispatcher.dispatch("print", {"Hello World", 42});

  // Example 2: string return type
  std::cout << "\n2. String return type:" << std::endl;
  MethodDispatcher<Request, std::string> stringDispatcher;
  
  stringDispatcher.registerHandler("process", [](const Request& req) -> std::string {
    return "Processed: " + req.data + " (value: " + std::to_string(req.value) + ")";
  });
  
  auto result1 = stringDispatcher.dispatch("process", {"Test Data", 123});
  if (result1) {
    std::cout << "  Result: " << *result1 << std::endl;
  }

  // Example 3: int return type with fallback
  std::cout << "\n3. Int return type with fallback:" << std::endl;
  MethodDispatcher<Request, int> intDispatcher;
  
  intDispatcher.registerHandler("calculate", [](const Request& req) -> int {
    return req.value * 2;
  });
  
  intDispatcher.setFallback([](const Request& req) -> int {
    std::cout << "  Using fallback for: " << req.data << std::endl;
    return -1;
  });
  
  auto result2 = intDispatcher.dispatch("calculate", {"Math", 10});
  if (result2) {
    std::cout << "  Calculate result: " << *result2 << std::endl;
  }
  
  auto result3 = intDispatcher.dispatch("unknown", {"Unknown", 5});
  if (result3) {
    std::cout << "  Fallback result: " << *result3 << std::endl;
  }

  // Example 4: No handler found
  std::cout << "\n4. No handler found:" << std::endl;
  auto result4 = stringDispatcher.dispatch("nonexistent", {"Test", 1});
  if (!result4) {
    std::cout << "  No result returned (as expected)" << std::endl;
  }

  return 0;
}