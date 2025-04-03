#include "loader/loader.h"
#include "util/util.h"
#include "picosystem.hpp"

#define VERSION     "0.1"
#define STARTUP_MSG "Loader " VERSION

using namespace picosystem;

ApplicationList<MAX_APPS> apps;

void Loader::init() {
  startup_timeout = Timeout(500);
}

void Loader::update(uint32_t tick) {
  if (pressed(UP) && pressed(X)) {
    flags.run_app = false;
  }

  if (flags.run_app) {
    apps.buffer[app_idx].app->update(tick);
  } else {
    if (pressed(Y)) {
      flags.draw_info = !flags.draw_info;
    }

    if (pressed(UP)) {
      app_idx = cap<int32_t>(app_idx - 1, 0, apps.size - 1);
    }

    if (pressed(DOWN)) {
      app_idx = cap<int32_t>(app_idx + 1, 0, apps.size - 1);
    }

    if (pressed(B)) {
      apps.buffer[app_idx].app->init();
      flags.run_app = true;
    }
  }
}

void Loader::draw(uint32_t tick) {
  pen(0, 0, 0);
  clear();

  if (!startup_timeout.expired()) {
    draw_startup_msg();
    return;
  }

  if (flags.run_app) {
    apps.buffer[app_idx].app->draw(tick);
  } else {
    draw_apps();
  }

  if (flags.draw_info) {
    draw_info();
  }
}

void Loader::draw_startup_msg() {
  int32_t x, y;

  measure(STARTUP_MSG, x, y);

  pen(0, 0, 0);
  clear();

  pen(0xF, 0xF, 0xF);
  text(STARTUP_MSG, (SCREEN->w - x) / 2, (SCREEN->h - y) / 2);
}

void Loader::draw_apps() {
  pen(0, 0, 0xF);
  text("Apps (press B to run):\n");

  pen(0xF, 0xF, 0xF);
  for (size_t i = 0; i < apps.size; ++i) {
    if (app_idx == i) {
      pen(0x8, 0xF, 0x8);
    }
    text( std::string(apps.buffer[i].name) + "\n");
    if (app_idx == i) {
      pen(0xF, 0xF, 0xF);
    }
  }
}

void Loader::draw_info() {
  auto bat_str = std::to_string(battery()) + "%";
  int32_t x, y;
  measure(bat_str, x, y);

  pen(0xF, 0xF, 0xF);
  text(bat_str, SCREEN->w - x - 1, SCREEN->h - y - 1);

  auto fps_str = std::to_string(stats.fps);
  measure(fps_str, x, y);
  text(fps_str, 1, SCREEN->h - y - 1);
}
