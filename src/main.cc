#include "picosystem.hpp"
#include "loader/loader.h"

// text("yulia is the best", x, y);

using namespace picosystem;

static Loader loader;

void init() {
  loader.init();
}

void update(uint32_t tick) {
  loader.update(tick);
}

void draw(uint32_t tick) {
  loader.draw(tick);
}