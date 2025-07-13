#pragma once

#include <string_view>
#include <type_traits>

namespace simple_enum {

namespace detail {

template <auto V> constexpr std::string_view n() noexcept {
  static_assert(std::is_enum_v<decltype(V)>, "simple_enum requires enum type");

#if defined(__clang__)
  constexpr auto func_name = __PRETTY_FUNCTION__;
  constexpr auto func_size = sizeof(__PRETTY_FUNCTION__);

  // Clang格式: "std::string_view simple_enum::detail::n() [V = Color::RED]"
  constexpr auto find_equals = [](const char *str, std::size_t size) {
    for (std::size_t i = 0; i < size - 2; ++i) {
      if (str[i] == '=' && str[i + 1] == ' ') {
        return i + 2;
      }
    }
    return std::size_t{0};
  };

  constexpr auto find_bracket = [](const char *str, std::size_t size) {
    for (std::size_t i = size - 1; i > 0; --i) {
      if (str[i] == ']') {
        return i;
      }
    }
    return size;
  };

  constexpr auto start_pos = find_equals(func_name, func_size);
  constexpr auto end_pos = find_bracket(func_name, func_size);

  if constexpr (start_pos > 0 && end_pos > start_pos) {
    constexpr auto name_size = end_pos - start_pos;
    constexpr auto name_data = func_name + start_pos;

    // 检查是否是有效的枚举名称（不是数字开头）
    if constexpr (name_size > 0 && name_data[0] != '(' && name_data[0] != '-' &&
                  !(name_data[0] >= '0' && name_data[0] <= '9')) {
      return std::string_view{name_data, name_size};
    }
  }

#elif defined(__GNUC__)
  constexpr auto func_name = __PRETTY_FUNCTION__;
  constexpr auto func_size = sizeof(__PRETTY_FUNCTION__);

  // GCC格式: "constexpr std::string_view simple_enum::detail::n() [with auto V = Color::RED;
  // std::string_view = std::basic_string_view<char>]" 我们需要提取 "Color::RED" 部分，忽略后面的类型信息

  constexpr auto find_equals = [](const char *str, std::size_t size) {
    for (std::size_t i = 0; i < size - 2; ++i) {
      if (str[i] == ' ' && str[i + 1] == '=' && str[i + 2] == ' ') {
        return i + 3;
      }
    }
    return std::size_t{0};
  };

  constexpr auto find_semicolon_or_bracket = [](const char *str, std::size_t size, std::size_t start) {
    for (std::size_t i = start; i < size; ++i) {
      if (str[i] == ';' || str[i] == ']') {
        return i;
      }
    }
    return size;
  };

  constexpr auto start_pos = find_equals(func_name, func_size);
  constexpr auto end_pos = find_semicolon_or_bracket(func_name, func_size, start_pos);

  if constexpr (start_pos > 0 && end_pos > start_pos) {
    constexpr auto name_size = end_pos - start_pos;
    constexpr auto name_data = func_name + start_pos;

    // 检查是否是有效的枚举名称
    if constexpr (name_size > 0 && name_data[0] != '(') {
      return std::string_view{name_data, name_size};
    }
  }

#elif defined(_MSC_VER)
  constexpr auto func_size = sizeof(__FUNCSIG__);

  // MSVC格式: 可能需要根据实际输出调整
  constexpr auto find_angle_start = [](const char *str, std::size_t size) {
    for (std::size_t i = 0; i < size - 1; ++i) {
      if (str[i] == '<') {
        return i + 1;
      }
    }
    return std::size_t{0};
  };

  constexpr auto find_angle_end = [](const char *str, std::size_t size) {
    for (std::size_t i = size - 1; i > 0; --i) {
      if (str[i] == '>') {
        return i;
      }
    }
    return size;
  };

  constexpr auto start_pos = find_angle_start(func_sig, func_size);
  constexpr auto end_pos = find_angle_end(func_sig, func_size);

  if constexpr (start_pos > 0 && end_pos > start_pos) {
    constexpr auto name_size = end_pos - start_pos;
    constexpr auto name_data = func_sig + start_pos;

    if constexpr (name_size > 0) {
      return std::string_view{name_data, name_size};
    }
  }
#endif

  return std::string_view{};
}

} // namespace detail

template <auto V> constexpr std::string_view enum_name() noexcept {
  static_assert(std::is_enum_v<decltype(V)>, "V must be an enum value");
  return detail::n<V>();
}

} // namespace simple_enum