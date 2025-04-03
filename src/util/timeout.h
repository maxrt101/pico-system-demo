#pragma once

#include "picosystem.hpp"

struct Timeout {
  uint32_t start;
  uint32_t duration;

  Timeout() : start(0), duration(0) {}

  explicit Timeout(uint32_t duration) : start(picosystem::time()), duration(duration) {}

  bool expired() const {
    return picosystem::time() > start + duration;
  }

  void expire() {
    start = 0;
  }

  void restart() {
    start = picosystem::time();
  }
};


