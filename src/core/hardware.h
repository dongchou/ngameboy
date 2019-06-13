#pragma once

#include <cstddef>
#include <cstdint>

namespace GB {
typedef uint8_t byte;

typedef uint8_t reg8_t;
typedef uint16_t reg16_t;
typedef int8_t r8_t;
typedef uint8_t d8_t;
typedef uint16_t d16_t;
typedef uint8_t a8_t;
typedef uint16_t a16_t;
typedef uint16_t vec_t;

class non_copyable {
protected:
  non_copyable() = default;
  ~non_copyable() = default;

  non_copyable(const non_copyable &) = delete;
  non_copyable &operator=(const non_copyable &x) = delete;
};

} // namespace GB