#pragma once
#include <cstdint>
#include "utils.h"
#include "gui.h"
#include <raylib.h>
#include <raymath.h>
#include <assert.h>
#define WORLD_TILE_DIM 4096
#define WORLD_TILE_SIZE 32

enum GParticleKind {
    GPARTICLE_PARTICLE, GPARTICLE_LINE, GPARTICLE_SQUARE,
};

struct GParticle {
    g_float2 pos = { 0.0, 0.0 };
    g_float2 pos2 = { 0.0, 0.0 };
    float width = 0.;
    float height = 0.;
    float radius = 0.;
    float rotation = 0.;
    g_float2 velocity = { 0.0, 0.0 };
    GParticleKind kind = GPARTICLE_PARTICLE;
    Color color = RED;
    float remaining_lifetime = 0.;
    bool exists = false;
};

struct GBounds {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
};

enum GEntityKind {
    GENTITY_KIND_NONE,
    GENTITY_KIND_PLAYER,
    GENTITY_KIND_ENEMY,
};

struct GEntity {
public:
    GEntityKind kind = GENTITY_KIND_NONE;
    string name = "";
    g_float2 position = { 0., 0. };
    g_float2 velocity = { 0., 0. };
    g_float2 facing = { 1., 0. };
    float width = 1.;
    float height = 1.;
    int32_t health = 1;
    float movement_speed = 80.;
    bool is_player = false;;
    void on_tick(float delta_time);
    void on_tick_paused(float delta_time);
    void on_render();
    GBounds get_bounds()const;
    void handle_movement(float delta_time, g_float2 input_direction);
    void fire_weapon();
};


struct GTile {
    bool is_occupied;
};
struct GWorld {
    unique_ptr<array <array<GTile, WORLD_TILE_DIM>, WORLD_TILE_DIM>> tiles;
    inline GTile& operator[](int32_t x, int32_t y) {
        assert(0 <= x && 0 <= y && x < (*tiles)[0].size() && y < (*tiles).size());
        return (*tiles)[y][x];
    }
    inline const GTile& operator[](int32_t x, int32_t y)const {
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

struct GRaycastResult {
    bool hit = false;
    g_float2 pos = { 0.0, 0.0 };
    g_float2 normal = { 0., 0. };
    GEntity* hit_entity = nullptr;
};
struct GGame {
    bool is_running;
    bool should_exit;
    bool is_paused;
    GWorld world;
    GPtrSet<GEntity> entities;
    vector<GEntity* > destroy_queue;
    vector<GParticle> particles;
    GUI gui;
    bool check_collision_rect(GBounds b);
    GRaycastResult raycast(g_float2 start, g_float2 end, const vector<GEntity*>& ignored_entities);
};

extern GGame game;

extern constexpr Vector2 to_rl(g_float2 p);
extern constexpr g_float2 to_game(Vector2 p);
GGame* get_game();
GWorld* get_world();
vector<GEntity*> get_entities();
GUI* get_gui();
void game_update();
void game_load_world(string_view path);
void game_teardown();
void menu_teardown();
void gameloop();
void menu_update();
void game_render();
void menu_render();
GWorld game_generate_world();
GEntity* new_entity(GEntity et);
void game_setup(GEntity* previous_player);

void spawn_particle(GParticle particle);

void particle_updates(float delta_time);
void particle_rendering(float delta_time);