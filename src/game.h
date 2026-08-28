#pragma once
#include <cstdint>
#include "utils.h"
#include "gui.h"
#include <raylib.h>
#include <assert.h>
#define WORLD_TILE_DIM 128
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
enum WeaponKind {
    WEAPON_KIND_NONE,
    WEAPON_KIND_KNIFE,
    WEAPON_KIND_CONVENTIONAL_PISTOL,
    WEAPON_KIND_CONVENTIONAL_RIFLE,
    WEAPON_KIND_CONVENTIONAL_SHOTGUN,
    WEAPON_KIND_GUASS_PISTOL,
    WeAPON_KIND_GUASS_RIFLE,
    WEAPON_KIND_GUASS_SMG,
    WEAPON_KIND_COUNT
};
struct Entity {
public:
    EntityKind kind;
    str name;
    float2 position;
    float2 velocity;
    float2 facing;
    float width;
    float height;
    int32_t health;
    int32_t ammo[WEAPON_KIND_COUNT][2];
    bool is_player;
    void on_tick(float delta_time);
    void on_tick_paused(float delta_time);
    void on_render();
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
struct Game {
    bool is_running;
    bool should_exit;
    bool is_paused;
    World world;
    PtrSet<Entity> entities;
    vector<Entity* > destroy_queue;
    GUI gui;
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