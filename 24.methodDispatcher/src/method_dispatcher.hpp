#pragma once

#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

template <typename ParamType, typename ReturnType = void>
class MethodDispatcher {
public:
  using Handler = std::function<ReturnType(const ParamType &)>;

  void registerHandler(const std::string &method, Handler &&handler) {
    handlers_[method] = std::move(handler);
  }

  void setFallback(Handler &&fallback) { fallback_ = std::move(fallback); }

  auto dispatch(const std::string &method, const ParamType &param) const {
    auto it = handlers_.find(method);
    if (it != handlers_.end()) {
      if constexpr (std::is_void_v<ReturnType>) {
        it->second(param);
      } else {
        return std::optional<ReturnType>{it->second(param)};
      }
    } else if (fallback_) {
      if constexpr (std::is_void_v<ReturnType>) {
        fallback_(param);
      } else {
        return std::optional<ReturnType>{fallback_(param)};
      }
    } else {
      std::cerr << "[Dispatcher] No handler for method: " << method << std::endl;
      if constexpr (!std::is_void_v<ReturnType>) {
        return std::optional<ReturnType>{};
      }
    }
  }

private:
  std::unordered_map<std::string, Handler> handlers_;
  Handler fallback_;
};