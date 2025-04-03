#include "picosystem.hpp"
#include "loader/loader.h"
#include "util/util.h"
#include "util/util.h"
#include <cstring>

using namespace picosystem;

struct Geometry : App {
  Vec2<int> pos;
  Vec2<int> clicked_pos;

  enum {IDLE, CLICKED, DONE} state;
  enum {DRAW, ERASE} action_state;
  enum {LINE, RECT, FRECT, ELIPSIS, FELIPSIS} figure;

  buffer_t buf;
  color_t data[SCREEN_SIZE * SCREEN_SIZE];

  union {
    uint8_t value;
    struct {
      bool draw_stat : 1;
    };
  } flags;

  Geometry() {
    buffer_init(&buf, 0, 0, nullptr);
  }

  void init() {
    pos = {0, 0};
    clicked_pos = {0, 0};
    state = IDLE;
    action_state = DRAW;
    figure = LINE;
    flags.value = 0;

    if (!buf.data) {
      buffer_init(&buf, SCREEN_SIZE, SCREEN_SIZE, data);
    }
  }

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

    if (pressed(B)) {
      if (state == IDLE) {
        clicked_pos = pos;
        state = CLICKED;
      } else if (state == CLICKED) {
        state = DONE;
      } else {
        state = IDLE;
      }
    }

    if (pressed(A)) {
      cycle_action_state();
    }

    if (pressed(Y)) {
      cycle_figure();
    }

    if (pressed(X)) {
      flags.draw_stat = !flags.draw_stat;
    }
  }

  void draw(uint32_t tick) {
    if (state == DONE) {
      target(&buf);
      set_color();
      draw_figure();
      target();
      state = IDLE;
    }

    blit(&buf, 0, 0, buf.w, buf.h, 0, 0);

    if (state == CLICKED) {
      set_color();
      draw_figure();
    }

    if (flags.draw_stat) {
      draw_stat();
    }
    
    pen(0x8, 0xF, 0x8, 0xF);
    pixel(pos.x, pos.y);
  }

private:
  void cycle_action_state() {
    action_state = action_state == DRAW ? ERASE : DRAW;
  }

  void cycle_figure() {
    switch (figure) {
      case LINE:
        figure = RECT;
        break;
      case RECT:
        figure = FRECT;
        break;
      case FRECT:
        figure = ELIPSIS;
        break;
      case ELIPSIS:
        figure = FELIPSIS;
        break;
      case FELIPSIS:
      default:
        figure = LINE;
        break;
    }
  }

  void draw_figure() {
    switch (figure) {
      case LINE:
        line(clicked_pos.x, clicked_pos.y, pos.x, pos.y);
        break;
      case RECT:
        case FRECT:
        (figure == FRECT ? frect : rect)(
          std::min(clicked_pos.x, pos.x),
          std::min(clicked_pos.y, pos.y),
          abs_diff(pos.x, clicked_pos.x),
          abs_diff(pos.y, clicked_pos.y)
        );
        break;
      case ELIPSIS:
        case FELIPSIS:
        (figure == FELIPSIS ? fellipse : ellipse)(
          std::min(clicked_pos.x, pos.x),
          std::min(clicked_pos.y, pos.y),
          abs_diff(pos.x, clicked_pos.x),
          abs_diff(pos.y, clicked_pos.y)
        );
        break;
      default:
        break;
    }
  }

  std::string action_state_to_str() const {
    switch (action_state) {
      case DRAW:
        return "D";
      case ERASE:
        return "E";
      default:
        return "?";
    }
  }

  std::string figure_to_str() const {
    switch (figure) {
      case LINE:
        return "L";
      case RECT:
        return "R";
      case FRECT:
        return "FR";
      case ELIPSIS:
        return "E";
      case FELIPSIS:
        return "FE";
      default:
        return "?";
    }
  }

  void draw_stat() {
    pen(0xF, 0xF, 0xF);
    text(action_state_to_str() + " " + figure_to_str(), 5, 5);
  }

  void set_color() const {
    if (action_state == DRAW) {
      pen(0xF, 0xF, 0xF);
    } else if (action_state == ERASE) {
      pen(0, 0, 0);
    } else {
      pen(0xF, 0, 0);
    }
  }
};

static Geometry geometry;

APP(Geometry, &geometry);
