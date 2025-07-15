#pragma once

#include <array>
#include <cassert>
#include <limits>
#include <optional>
#include <string_view>

// Enum value must be greater or equals than MAGIC_ENUM_RANGE_MIN. By default MAGIC_ENUM_RANGE_MIN =
// -128. If need another min range for all enum types by default, redefine the macro
// MAGIC_ENUM_RANGE_MIN.
#if !defined(MAGIC_ENUM_RANGE_MIN)
#define MAGIC_ENUM_RANGE_MIN -128
#endif

// Enum value must be less or equals than MAGIC_ENUM_RANGE_MAX. By default MAGIC_ENUM_RANGE_MAX = 127.
// If need another max range for all enum types by default, redefine the macro MAGIC_ENUM_RANGE_MAX.
#if !defined(MAGIC_ENUM_RANGE_MAX)
#define MAGIC_ENUM_RANGE_MAX 127
#endif

namespace simple_enum {
namespace detail {

template <auto V> constexpr std::string_view enum_string() noexcept {
  static_assert(std::is_enum_v<decltype(V)>, "enum requires enum type");

#if defined(__clang__)
  constexpr auto func_name = __PRETTY_FUNCTION__;
  constexpr auto func_size = sizeof(__PRETTY_FUNCTION__);

  // Clang
  // "std::string_view simple_enum::detail::enum_string() [V = Color::RED]"
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

    // check if the name is valid (not a number)
    if constexpr (name_size > 0 && name_data[0] != '(' && name_data[0] != '-' &&
                  !(name_data[0] >= '0' && name_data[0] <= '9')) {
      return std::string_view{name_data, name_size};
    }
  }

#elif defined(__GNUC__)
  constexpr auto func_name = __PRETTY_FUNCTION__;
  constexpr auto func_size = sizeof(__PRETTY_FUNCTION__);

  // GCC
  // "constexpr std::string_view simple_enum::detail::enum_string() [with auto V = Color::RED;
  // std::string_view = std::basic_string_view<char>]"
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

    // check if the name is valid (not a number)
    if constexpr (name_size > 0 && name_data[0] != '(') {
      return std::string_view{name_data, name_size};
    }
  }
#endif

  return std::string_view{};
}

// CWG1766: Values outside the range of the values of an enumeration
// https://reviews.llvm.org/D130058, https://reviews.llvm.org/D131307
#if defined(__clang__) && __clang_major__ >= 16
template <typename E, auto V, typename = void>
inline constexpr bool is_enum_constexpr_static_cast_valid = false;
template <typename E, auto V>
inline constexpr bool is_enum_constexpr_static_cast_valid<
    E, V, std::void_t<std::integral_constant<E, static_cast<E>(V)>>> = true;
#else
template <typename, auto> inline constexpr bool is_enum_constexpr_static_cast_valid = true;
#endif

template <typename E, int O, typename U = std::underlying_type_t<E>>
constexpr U ualue(std::size_t i) noexcept {
  if constexpr (std::is_same_v<U, bool>) { // bool special case
    static_assert(O == 0, "detail::ualue requires valid offset.");
    return static_cast<U>(i);
  } else {
    return static_cast<U>(static_cast<int>(i) + O);
  }
}

template <typename E, int O, typename U = std::underlying_type_t<E>>
constexpr E value(std::size_t i) noexcept {
  return static_cast<E>(ualue<E, O>(i));
}

template <typename E, auto V> constexpr bool is_valid() noexcept {
  if constexpr (is_enum_constexpr_static_cast_valid<E, V>) {
    constexpr E v = static_cast<E>(V);
    return enum_string<v>().size() != 0;
  } else {
    return false;
  }
}

// clang-format off
#define MAGIC_ENUM_FOR_EACH_256(T)                                                                                                                                                                 \
  T(  0)T(  1)T(  2)T(  3)T(  4)T(  5)T(  6)T(  7)T(  8)T(  9)T( 10)T( 11)T( 12)T( 13)T( 14)T( 15)T( 16)T( 17)T( 18)T( 19)T( 20)T( 21)T( 22)T( 23)T( 24)T( 25)T( 26)T( 27)T( 28)T( 29)T( 30)T( 31) \
  T( 32)T( 33)T( 34)T( 35)T( 36)T( 37)T( 38)T( 39)T( 40)T( 41)T( 42)T( 43)T( 44)T( 45)T( 46)T( 47)T( 48)T( 49)T( 50)T( 51)T( 52)T( 53)T( 54)T( 55)T( 56)T( 57)T( 58)T( 59)T( 60)T( 61)T( 62)T( 63) \
  T( 64)T( 65)T( 66)T( 67)T( 68)T( 69)T( 70)T( 71)T( 72)T( 73)T( 74)T( 75)T( 76)T( 77)T( 78)T( 79)T( 80)T( 81)T( 82)T( 83)T( 84)T( 85)T( 86)T( 87)T( 88)T( 89)T( 90)T( 91)T( 92)T( 93)T( 94)T( 95) \
  T( 96)T( 97)T( 98)T( 99)T(100)T(101)T(102)T(103)T(104)T(105)T(106)T(107)T(108)T(109)T(110)T(111)T(112)T(113)T(114)T(115)T(116)T(117)T(118)T(119)T(120)T(121)T(122)T(123)T(124)T(125)T(126)T(127) \
  T(128)T(129)T(130)T(131)T(132)T(133)T(134)T(135)T(136)T(137)T(138)T(139)T(140)T(141)T(142)T(143)T(144)T(145)T(146)T(147)T(148)T(149)T(150)T(151)T(152)T(153)T(154)T(155)T(156)T(157)T(158)T(159) \
  T(160)T(161)T(162)T(163)T(164)T(165)T(166)T(167)T(168)T(169)T(170)T(171)T(172)T(173)T(174)T(175)T(176)T(177)T(178)T(179)T(180)T(181)T(182)T(183)T(184)T(185)T(186)T(187)T(188)T(189)T(190)T(191) \
  T(192)T(193)T(194)T(195)T(196)T(197)T(198)T(199)T(200)T(201)T(202)T(203)T(204)T(205)T(206)T(207)T(208)T(209)T(210)T(211)T(212)T(213)T(214)T(215)T(216)T(217)T(218)T(219)T(220)T(221)T(222)T(223) \
  T(224)T(225)T(226)T(227)T(228)T(229)T(230)T(231)T(232)T(233)T(234)T(235)T(236)T(237)T(238)T(239)T(240)T(241)T(242)T(243)T(244)T(245)T(246)T(247)T(248)T(249)T(250)T(251)T(252)T(253)T(254)T(255)
// clang-format on

template <typename E, std::size_t Size, int Min, std::size_t I>
constexpr void valid_count(bool *valid, std::size_t &count) noexcept {
#define MAGIC_ENUM_V(O) \
  if constexpr ((I + O) < Size) { \
    if constexpr (is_valid<E, ualue<E, Min>(I + O)>()) { \
      valid[I + O] = true; \
      ++count; \
    } \
  }

  MAGIC_ENUM_FOR_EACH_256(MAGIC_ENUM_V)

  if constexpr ((I + 256) < Size) {
    valid_count<E, Size, Min, I + 256>(valid, count);
  }
#undef MAGIC_ENUM_V
}

template <std::size_t N> struct valid_count_t {
  std::size_t count = 0;
  bool valid[N] = {};
};

template <typename E, std::size_t Size, int Min> constexpr auto valid_count() noexcept {
  valid_count_t<Size> vc;
  valid_count<E, Size, Min, 0>(vc.valid, vc.count);
  return vc;
}

template <typename L, typename R> constexpr bool cmp_less(L lhs, R rhs) noexcept {
  static_assert(std::is_integral_v<L> && std::is_integral_v<R>,
                "magic_enum::detail::cmp_less requires integral type.");

  if constexpr (std::is_signed_v<L> == std::is_signed_v<R>) {
    // If same signedness (both signed or both unsigned).
    return lhs < rhs;
  } else if constexpr (std::is_same_v<L, bool>) { // bool special case
    return static_cast<R>(lhs) < rhs;
  } else if constexpr (std::is_same_v<R, bool>) { // bool special case
    return lhs < static_cast<L>(rhs);
  } else if constexpr (std::is_signed_v<R>) {
    // If 'right' is negative, then result is 'false', otherwise cast & compare.
    return rhs > 0 && lhs < static_cast<std::make_unsigned_t<R>>(rhs);
  } else {
    // If 'left' is negative, then result is 'true', otherwise cast & compare.
    return lhs < 0 || static_cast<std::make_unsigned_t<L>>(lhs) < rhs;
  }
}

template <typename T, typename = void>
struct range_min : std::integral_constant<int, MAGIC_ENUM_RANGE_MIN> {};

template <typename E, typename U = std::underlying_type_t<E>> constexpr int reflected_min() noexcept {
  constexpr auto lhs = range_min<E>::value;
  constexpr auto rhs = (std::numeric_limits<U>::min)();

  if constexpr (cmp_less(rhs, lhs)) {
    return lhs;
  } else {
    return rhs;
  }
}

template <typename T, typename = void>
struct range_max : std::integral_constant<int, MAGIC_ENUM_RANGE_MAX> {};

template <typename E, typename U = std::underlying_type_t<E>> constexpr int reflected_max() noexcept {
  constexpr auto lhs = range_max<E>::value;
  constexpr auto rhs = (std::numeric_limits<U>::max)();

  if constexpr (cmp_less(lhs, rhs)) {
    return lhs;
  } else {
    return rhs;
  }
}

template <typename E, std::size_t Size, int Min> constexpr auto values() noexcept {
  constexpr auto vc = valid_count<E, Size, Min>();

  if constexpr (vc.count > 0) {
    std::array<E, vc.count> values = {};
    for (std::size_t i = 0, v = 0; v < vc.count; ++i) {
      if (vc.valid[i]) {
        values[v++] = value<E, Min>(i);
      }
    }
    return values;
  } else {
    return std::array<E, 0>{};
  }
}

template <typename E> constexpr auto values() noexcept {
  constexpr auto min = reflected_min<E>();
  constexpr auto max = reflected_max<E>();
  constexpr auto range_size = max - min + 1;
  static_assert(range_size > 0, "magic_enum::enum_range requires valid size.");

  return values<E, range_size, min>();
}

template <typename E> inline constexpr auto values_v = values<E>();

template <typename E> inline constexpr auto count_v = values_v<E>.size();

template <typename E, std::size_t... I> constexpr auto names(std::index_sequence<I...>) noexcept {
  constexpr auto names = std::array<std::string_view, sizeof...(I)>{{enum_string<values_v<E>[I]>()...}};
  return names;
}

template <typename E> inline constexpr auto names_v = names<E>(std::make_index_sequence<count_v<E>>{});

template <typename E> inline constexpr bool is_reflected_v = std::is_enum_v<E> && (count_v<E> != 0);

// Returns enum value at specified index.
// No bounds checking is performed: the behavior is undefined if index >= number of enum values.
template <typename E>
[[nodiscard]] constexpr auto enum_value(std::size_t index) noexcept -> std::decay_t<E> {
  using D = std::decay_t<E>;
  static_assert(is_reflected_v<D>, "requires enum implementation and valid max and min.");
  return assert(index < count_v<D>), values_v<D>[index];
}

// Obtains index in enum values from enum value.
// Returns optional with index.
template <typename E> [[nodiscard]] constexpr std::optional<std::size_t> enum_index(E value) noexcept {
  using D = std::decay_t<E>;
  static_assert(is_reflected_v<D>, "requires enum implementation and valid max and min.");
  for (std::size_t i = 0; i < count_v<D>; ++i) {
    if (enum_value<D>(i) == value) {
      return i;
    }
  }
  return {}; // Invalid value or out of range.
}

} // namespace detail

// Returns name from enum value.
// If enum value does not have name or value out of range, returns empty string.
template <typename E> [[nodiscard]] constexpr std::string_view enum_name(E value) noexcept {
  using D = std::decay_t<E>;
  static_assert(detail::is_reflected_v<D>, "requires enum implementation and valid max and min.");

  if (const auto i = detail::enum_index<D>(value)) {
    return detail::names_v<D>[*i];
  }
  return {};
}

} // namespace simple_enum