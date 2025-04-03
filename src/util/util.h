#pragma once

#include "picosystem.hpp"
#include "util/vec2.h"
#include "util/vec3.h"
#include "util/math.h"
#include "util/bitmap.h"
#include "util/timeout.h"

#ifdef PIXEL_DOUBLE
#define SCREEN_SIZE 120
#else
#define SCREEN_SIZE 240
#endif

inline void buffer_init(picosystem::buffer_t * buffer, int32_t w, int32_t h, picosystem::color_t * data) {
  if (!buffer || !data) {
    return;
  }

  buffer->w = w;
  buffer->h = h;
  buffer->data = data;
  buffer->alloc = false;
}
