#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

template <size_t N>
struct Bitmap {
  uint8_t buffer[N / 8];

  size_t size() const {
    return N;
  }

  void clear() {
    memset(buffer, 0, N / 8);
  }

  bool get(uint32_t i) {
    return buffer[i / 8] & (1 << (i % 8));
  }

  void set(uint32_t i, bool value) {
    if (value) {
      buffer[i / 8] |= (1 << (i % 8));
    } else {
      buffer[i / 8] &= ~(1 << (i % 8));
    }
  }
};

