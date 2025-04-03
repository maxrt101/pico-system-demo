#include "picosystem.hpp"
#include "loader/loader.h"
#include "util/util.h"
#include <cstring>

using namespace picosystem;

template <size_t W, size_t H>
struct Map {
  Bitmap<W * H> bitmap;

  size_t width() const {
    return W;
  }

  size_t height() const {
    return H;
  }

  void clear() {
    bitmap.clear();
  }

  bool get(uint32_t x, uint32_t y) {
    return bitmap.get(y * H + x);
  }

  void set(uint32_t x, uint32_t y, bool value) {
    return bitmap.set(y * H + x, value);
  }
};

struct Player {
  Vec2<int> pos;

  void update(uint32_t tick) {
    if (button(UP)) {
      pos.y = cap<int32_t>(pos.y - 1, 0, SCREEN->h - 1);
    }

    if (button(DOWN)) {
      pos.y = cap<int32_t>(pos.y + 1, 0, SCREEN->h - 1);
    }

    if (button(LEFT)) {
      pos.x = cap<int32_t>(pos.x - 1, 0, SCREEN->w - 1);
    }

    if (button(RIGHT)) {
      pos.x = cap<int32_t>(pos.x + 1, 0, SCREEN->w - 1);
    }
  }

  void draw(uint32_t tick) {
    pen(0xF, 0, 0);
    pixel(pos.x, pos.y);
  }
};

struct Drawer : App {
  Player player;
  Map<SCREEN_SIZE, SCREEN_SIZE> map;

  void init() {
    player.pos = {0, 0};
    map.clear();
  }

  void update(uint32_t tick) {
    if (button(B)) {
      map.set(player.pos.x, player.pos.y, true);
    }

    if (button(A)) {
      map.set(player.pos.x, player.pos.y, false);
    }

    player.update(tick);
  }

  void draw(uint32_t tick) {
    player.draw(tick);

    pen(0, 0xF, 0xF);

    for (uint32_t y = 0; y < map.height(); ++y) {
      for (uint32_t x = 0; x < map.width(); ++x) {
        if (map.get(x, y)) {
          pixel(x, y);
        }
      }
    }

  }
};

static Drawer drawer;

APP(Drawer, &drawer);
