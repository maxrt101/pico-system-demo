#include "picosystem.hpp"
#include "loader/loader.h"
#include "util/util.h"
#include <cstring>
#include <cmath>

using namespace picosystem;

#define START_ANGLE         315.0f
#define BASE_UPDATE_ANGLE   180.0f
#define RAND_ANGLE_FRACTION 15

struct Object {
  Vec2<float> pos;
  float angle = START_ANGLE;

  void update(uint32_t tick) {
    pos.y = cap<float>(pos.y + sin(angle), 0, SCREEN->h - 1);
    pos.x = cap<float>(pos.x + cos(angle), 0, SCREEN->w - 1);

    if (pos.y <= 0) {
      angle += BASE_UPDATE_ANGLE + (tick % RAND_ANGLE_FRACTION);
    } else if (pos.y + 1 >= SCREEN->h) {
      angle -= BASE_UPDATE_ANGLE + (tick % RAND_ANGLE_FRACTION);
    } else if (pos.x <= 0) {
      angle += BASE_UPDATE_ANGLE + (tick % RAND_ANGLE_FRACTION);
    } else if (pos.x + 1 >= SCREEN->w) {
      angle -= BASE_UPDATE_ANGLE + (tick % RAND_ANGLE_FRACTION);
    }
  }

  void draw(uint32_t tick) {
    pen(0xF, 0, 0);
    pixel(pos.x, pos.y);
  }
};

struct Bounce : App {
  Object object;

  void init() {
    object.pos = {0, 0};
  }

  void update(uint32_t tick) {
    object.update(tick);
  }

  void draw(uint32_t tick) {
    object.draw(tick);
  }
};

static Bounce bounce;

APP(Bounce, &bounce);
