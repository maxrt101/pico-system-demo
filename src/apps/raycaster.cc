#include "picosystem.hpp"
#include "loader/loader.h"
#include "util/util.h"
#include <cstring>
#include <cmath>

#define MAP_SIZE                12
#define MAP_TILE_SCALE          (SCREEN_SIZE / MAP_SIZE)
#define TILE_EMPTY              0
#define TILE_WALL               1
#define USE_SAMPLE_MAP          1
#define USE_2D_MAP_RENDER       1
#define MAP_RENDER_SCALE        1

using namespace picosystem;

struct Player {
  Vec2<double> position       = {0, 0};
  Vec2<double> velocity       = {0, 0};
  float        angle          = 0.0f;
  float        rotation_speed = 0.2f;
  float        movement_speed = 0.2f;
};

template <int32_t N>
struct Map {
  struct Tile {
    uint8_t type;

    Tile(uint8_t type) : type(type) {}
  };

#if USE_SAMPLE_MAP
  Tile tiles[N * N] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  };
#else
  Tile tiles[N * N];
#endif

  Map() {
#if !USE_SAMPLE_MAP
    memset(&tiles, 0, sizeof(Tile) * N * N);
#endif
  }

  Map(Tile * tiles) {
    memcpy(this->tiles, tiles, sizeof(Tile) * N * N);
  }

  int32_t size() const {
    return N;
  }

  Tile get(int32_t index) {
    return tiles[index];
  }

  Tile get(int32_t x, int32_t y) {
    return tiles[y * N + x];
  }

  Tile get(Vec2<int> pos) {
    return get(pos.x, pos.y);
  }
};

enum Side {
  NORTH, SOUTH, WEST, EAST, TOP, BOTTOM
};

struct TileHit {
  Vec2<int>     tile_position = {0, 0};
  Vec2<double>  hit_position  = {0, 0};
  float         ray_length    = 0.0f;
  float         sample_x      = 0.0f;
  Side          side          = NORTH;
};

struct DDAResult {
  bool hit_wall = false;

  TileHit tile;
};

struct Raycaster : App {
  Player player;
  Map<MAP_SIZE> map;

  float mFov   = 3.14159f / 4.0f;
  float mDepth = 30.0f;
  float mStep  = 0.01f;

  void init() {
    player.position.x = map.size() / 2;
    player.position.y = map.size() / 2;
  }

  void update(uint32_t tick) {
    if (button(LEFT)) {
      player.angle -= player.rotation_speed; //* frameTime;
      if (player.angle < -2.0f * M_PI) { // kinda works?
        player.angle += 2.0f * M_PI;
      }
    }

    if (button(RIGHT)) {
      player.angle += player.rotation_speed; //* frameTime;
      if (player.angle > 2.0f * M_PI) { // kinda works?
        player.angle -= 2.0f * M_PI;
      }
    }

    if (button(UP)) {
      player.position.x += sinf(player.angle) * player.movement_speed; //* frameTime;
      player.position.y += cosf(player.angle) * player.movement_speed; //* frameTime;

      if (map.get((int) player.position.x, (int) player.position.y).type == TILE_WALL) {
        player.position.x -= sinf(player.angle) * player.movement_speed; //* frameTime;
        player.position.y -= cosf(player.angle) * player.movement_speed; //* frameTime;
      }
    }

    if (button(DOWN)) {
      player.position.x -= sinf(player.angle) * player.movement_speed; //* frameTime;
      player.position.y -= cosf(player.angle) * player.movement_speed; //* frameTime;

      if (map.get((int) player.position.x, (int) player.position.y).type == TILE_WALL) {
        player.position.x += sinf(player.angle) * player.movement_speed; //* frameTime;
        player.position.y += cosf(player.angle) * player.movement_speed; //* frameTime;
      }
    }
  }

  void draw(uint32_t tick) {
    auto screen_height = SCREEN->h;
    auto screen_width = SCREEN->w;

    for (int x = 0; x < screen_width; x++) {
      float ray_angle = (player.angle - mFov/2.0f) + (x / (float)screen_width) * mFov;
      Vec2<double> ray_direction = {sinf(ray_angle), cosf(ray_angle)};

      DDAResult result = cast_ray(player.position, ray_direction);

      if (result.hit_wall) {
        Vec2<double> ray = {
          result.tile.hit_position.x - player.position.x,
          result.tile.hit_position.y - player.position.y
        };

        float ray_length = sqrt(ray.x * ray.x + ray.y * ray.y) * cos(ray_angle - player.angle);

        float ceiling = (screen_height / 2.0f) - screen_height / ray_length;
        float floor = screen_height - ceiling;
        float wall_height = cap<float>(floor - ceiling, 0, screen_height);

#if 0
        float whole;
        int texture_x = std::modf(result.tile.sampleX, &whole) * texture->getWidth();
#endif

        int32_t color = scale(
          wall_height,
          {0, (float) screen_height},
          {1, 0xF}
        );

        pen(color, color, color, 0xF);
        vline(x, cap<float>(ceiling, 1, screen_height), wall_height);
      }
    }

#if USE_2D_MAP_RENDER
    pen(0x8, 0x8, 0xF);
    for (int32_t y = 0; y < map.size(); ++y) {
      for (int32_t x = 0; x < map.size(); ++x) {
        if (map.get(x, y).type == TILE_WALL) {
          rect(x * MAP_RENDER_SCALE, y * MAP_RENDER_SCALE, MAP_RENDER_SCALE, MAP_RENDER_SCALE);
        }
      }
    }

    pen(0xF, 0, 0);
    pixel(player.position.x, player.position.y);
#endif
  }

private:
  DDAResult cast_ray(Vec2<double> src, Vec2<double> direction) {
    DDAResult result;

    Vec2<double> ray_delta = {
      sqrt(1 + (direction.y / direction.x) * (direction.y / direction.x)),
      sqrt(1 + (direction.x / direction.y) * (direction.x / direction.y))
    };

    Vec2<int> map_check = {(int)src.x, (int)src.y};
    Vec2<double> side_distance, step_distance;

    if (direction.x < 0) {
      step_distance.x = -1;
      side_distance.x = (src.x - (float)map_check.x) * ray_delta.x;
    } else {
      step_distance.x = 1;
      side_distance.x = ((float)map_check.x + 1 - src.x) * ray_delta.x;
    }

    if (direction.y < 0) {
      step_distance.y = -1;
      side_distance.y = (src.y - (float)map_check.y) * ray_delta.y;
    } else {
      step_distance.y = 1;
      side_distance.y = ((float)map_check.y + 1 - src.y) * ray_delta.y;
    }

    Vec2<double> intersection;
    Vec2<int> hit_tile;
    float max_distance  = 100.0f;
    float distance      = 0.0f;
    TileHit hit;

    while (!result.hit_wall && distance < max_distance) {
      if (side_distance.x < side_distance.y) {
        side_distance.x += ray_delta.x;
        map_check.x += step_distance.x;
      } else {
        side_distance.y += ray_delta.y;
        map_check.y += step_distance.y;
      }

      Vec2<double> ray_distance = {(float)map_check.x - src.x, (float)map_check.y - src.y};
      distance = sqrt(ray_distance.x * ray_distance.x + ray_distance.y * ray_distance.y);

      if (map.get(map_check).type == TILE_WALL) {
        hit_tile = map_check;

        result.hit_wall = true;

        hit.tile_position = map_check;

        float m = direction.y / direction.x;

        if (src.y <= map_check.y) {
          if (src.x <= map_check.x) {
            hit.side = WEST;
            intersection.y = m * (map_check.x - src.x) + src.y;
            intersection.x = float(map_check.x);
            hit.sample_x = intersection.y - std::floor(intersection.y);
          } else if (src.x >= (map_check.x + 1)) {
            hit.side = EAST;
            intersection.y = m * ((map_check.x + 1) - src.x) + src.y;
            intersection.x = float(map_check.x + 1);
            hit.sample_x = intersection.y - std::floor(intersection.y);
          } else {
            hit.side = NORTH;
            intersection.y = float(map_check.y);
            intersection.x = (map_check.y - src.y) / m + src.x;
            hit.sample_x = intersection.x - std::floor(intersection.x);
          }

          if (intersection.y < map_check.y) {
            hit.side = NORTH;
            intersection.y = float(map_check.y);
            intersection.x = (map_check.y - src.y) / m + src.x;
            hit.sample_x = intersection.x - std::floor(intersection.x);
          }
        } else if (src.y >= map_check.y + 1) {
          if (src.x <= map_check.x) {
            hit.side = WEST;
            intersection.y = m * (map_check.x - src.x) + src.y;
            intersection.x = float(map_check.x);
            hit.sample_x = intersection.y - std::floor(intersection.y);
          } else if (src.x >= (map_check.x + 1)) {
            hit.side = EAST;
            intersection.y = m * ((map_check.x + 1) - src.x) + src.y;
            intersection.x = float(map_check.x + 1);
            hit.sample_x = intersection.y - std::floor(intersection.y);
          } else {
            hit.side = SOUTH;
            intersection.y = float(map_check.y + 1);
            intersection.x = ((map_check.y + 1) - src.y) / m + src.x;
            hit.sample_x = intersection.x - std::floor(intersection.x);
          }

          if (intersection.y > (map_check.y + 1)) {
            hit.side = SOUTH;
            intersection.y = float(map_check.y + 1);
            intersection.x = ((map_check.y + 1) - src.y) / m + src.x;
            hit.sample_x = intersection.x - std::floor(intersection.x);
          }
        } else {
          if (src.x <= map_check.x) {
            hit.side = WEST;
            intersection.y = m * (map_check.x - src.x) + src.y;
            intersection.x = float(map_check.x);
            hit.sample_x = intersection.y - std::floor(intersection.y);
          } else if (src.x >= (map_check.x + 1)) {
            hit.side = EAST;
            intersection.y = m * ((map_check.x + 1) - src.x) + src.y;
            intersection.x = float(map_check.x + 1);
            hit.sample_x = intersection.y - std::floor(intersection.y);
          }
        }

        hit.hit_position = intersection;

        result.tile = hit;
      }
    }

    return result;
  }
};

static Raycaster raycaster;

APP(Raycaster, &raycaster);
