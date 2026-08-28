#pragma once
#include <cstdint>
#include "utils.h"
#include "gui.h"
#include <raylib.h>
#include <assert.h>
#define WORLD_TILE_DIM 128
#define WORLD_TILE_SIZE 32
struct Bounds {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
};

enum EntityKind {
    ENTITY_KIND_NONE,
    ENTITY_KIND_PLAYER,
    ENTITY_KIND_ENEMY,
};

struct Entity {
public:
    EntityKind kind = ENTITY_KIND_NONE;
    string name = "";
    float2 position = { 0., 0. };
    float2 velocity = { 0., 0. };
    float2 facing = { 1., 0. };
    float width = 1.;
    float height = 1.;
    int32_t health = 1;
    bool is_player = false;;
    void on_tick(float delta_time);
    void on_tick_paused(float delta_time);
    void on_render();
    Bounds get_bounds()const;
};


struct Tile {
    bool is_occupied;
};
struct World {
    unique_ptr<array <array<Tile, WORLD_TILE_DIM>, WORLD_TILE_DIM>> tiles;
    inline Tile& operator[](int32_t x, int32_t y) {
        assert(0 <= x && 0 <= y && x < (*tiles)[0].size() && y < (*tiles).size());
        return (*tiles)[y][x];
    }
    inline const Tile& operator[](int32_t x, int32_t y)const {
        assert(0 <= x && 0 <= y && x < (*tiles)[0].size() && y < (*tiles).size());
        return (*tiles)[y][x];
    }
    inline int32_t width()const {
        return (*tiles)[0].size();
    }
    inline int32_t height() const {
        return (*tiles).size();
    }
};
struct RaycastResult {
    bool hit = false;
    float2 pos = { 0.0, 0.0 };
    float2 normal = { 0., 0. };
    Entity* hit_entity = nullptr;
};
struct Game {
    bool is_running;
    bool should_exit;
    bool is_paused;
    World world;
    PtrSet<Entity> entities;
    vector<Entity* > destroy_queue;
    GUI gui;
    bool check_collision_rect(Bounds b);
    RaycastResult raycast(float2 start, float2 end);
};

extern Game game;

extern constexpr Vector2 to_rl(float2 p);
extern constexpr float2 to_game(Vector2 p);
Game* get_game();
World* get_world();
vector<Entity*> get_entities();
GUI* get_gui();
void game_update();
void game_load_world(string_view path);
void game_teardown();
void menu_teardown();
void gameloop();
void menu_update();
void game_render();
void menu_render();
World game_generate_world();
Entity* new_entity(Entity et);
void game_setup(Entity* previous_player);


