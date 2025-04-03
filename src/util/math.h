#pragma once

#include "util/vec2.h"

template <typename T>
constexpr T abs_diff(T a, T b) {
  return a > b ? a - b : b - a;
}

template <typename T>
constexpr T cap(T value, T min, T max) {
  if (value < min) {
    return min;
  }

  if (value > max) {
    return max;
  }

  return value;
}

template <typename T, typename R = T>
R scale(T value, Vec2<T> val_constraints, Vec2<R> res_constraints) {
  return ((value - val_constraints.x) / (val_constraints.y - val_constraints.x)) * (res_constraints.y - res_constraints.x) + res_constraints.x;
}

// template <typename T>
// void updateChecked(T& val, T upd, Vec2<T> constraints) {
//   if (val + upd < constraints.x) {
//     val = constraints.x;
//   } else if (val + upd > constraints.y) {
//     val = constraints.y;
//   } else {
//     val += upd;
//   }
// }

