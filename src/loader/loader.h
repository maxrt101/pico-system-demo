#pragma once

#include "util/timeout.h"
#include <cstdint>
#include <cstddef>

#define MAX_APPS 5

#define APP(__name, __instance)                                 \
  __attribute__((constructor(255))) void __init_ ## __name() {  \
    apps.buffer[apps.size].name = #__name;                      \
    apps.buffer[apps.size].app = __instance;                    \
    apps.size++;                                                \
  }

struct App {
  virtual ~App() = default;

  virtual void init() = 0;
  virtual void update(uint32_t tick) = 0;
  virtual void draw(uint32_t tick) = 0;
};

struct Application {
  App * app;
  const char * name;
};

template <size_t N>
struct ApplicationList {
  Application buffer[N];
  size_t size;
  size_t capacity;
};

struct Loader {
  union {
    uint8_t value;
    struct {
      bool is_init    : 1;
      bool run_app    : 1;
      bool draw_info  : 1;
    };
  } flags;

  int32_t app_idx;
  Timeout startup_timeout;

  void init();
  void update(uint32_t tick);
  void draw(uint32_t tick);

  void draw_startup_msg();
  void draw_apps();
  void draw_info();
};

extern ApplicationList<MAX_APPS> apps;
