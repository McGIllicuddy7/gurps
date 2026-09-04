#pragma once
#include "gui.h"
#include "utils.h"
#include <assert.h>
#include <cstdint>
#include <raylib.h>
#include <raymath.h>
#define WORLD_TILE_DIM 512
#define WORLD_TILE_SIZE 32
enum GGameState {
        GGAME_STATE_MENU,
        GGAME_STATE_RUNNING,
        GGAME_STATE_PAUSED,
        GGAME_STATE_PLAYER_DEAD
};
enum GParticleKind {
        GPARTICLE_PARTICLE,
        GPARTICLE_LINE,
        GPARTICLE_SQUARE,
};

struct GParticle {
        g_float2 m_pos = {0.0, 0.0};
        g_float2 m_pos2 = {0.0, 0.0};
        float m_width = 0.;
        float m_height = 0.;
        float m_radius = 0.;
        float m_rotation = 0.;
        g_float2 m_velocity = {0.0, 0.0};
        GParticleKind m_kind = GPARTICLE_PARTICLE;
        Color m_color = RED;
        float m_remaining_lifetime = 0.;
        bool m_exists = false;
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

enum GEntityAction {
        GENTITY_ACTION_DEFAULT,
        GENTITY_ACTION_RELOAD,
        GENTITY_ACTION_RECOIL,
        GENTITY_ACTION_SWITCHING_WEAPONS,
};

enum GDamageType {
        G_DAMAGE_TYPE_CRUSHING,
        G_DAMAGE_TYPE_CUTTING,
        G_DAMAGE_TYPE_IMPALING,
};

struct GEntityActionData {
        GEntityAction current_action = GENTITY_ACTION_DEFAULT;
        float remaining_time = 0.;
};

struct GAiData {
        float m_desired_facing;
};
struct GEntity {
        GEntityKind m_kind = GENTITY_KIND_NONE;
        string m_name = "";
        g_float2 m_position = {0., 0.};
        g_float2 m_velocity = {0., 0.};
        g_float2 m_facing = {1., 0.};
        float m_width = 1.;
        float m_height = 1.;
        int32_t m_health = 1;
        float m_movement_speed = 80.;
        bool m_is_player = false;
        GAiData m_ai_data;
        GEntityActionData m_action_data;
        // functions;
        void on_tick(float delta_time);
        void on_tick_paused(float delta_time);
        void on_render();
        GBounds get_bounds() const;
        void handle_movement(float delta_time, g_float2 input_direction);
        void fire_weapon(float delta_time);
        void apply_damage(int32_t damage, GDamageType damage_type);
        void on_tick_player(float delta_time);
        void on_tick_enemy(float delta_time);
        void handle_actions(float delta_time);
};

struct GTile {
        bool is_occupied;
};
struct GWorld {
        unique_ptr<array<array<GTile, WORLD_TILE_DIM>, WORLD_TILE_DIM>> tiles;
        inline GTile &operator()(int32_t x, int32_t y) {
                assert(0 <= x && 0 <= y && x < (*tiles)[0].size() &&
                       y < (*tiles).size());
                return (*tiles)[y][x];
        }
        inline const GTile &operator()(int32_t x, int32_t y) const {
                assert(0 <= x && 0 <= y && x < (*tiles)[0].size() &&
                       y < (*tiles).size());
                return (*tiles)[y][x];
        }
        inline int32_t width() const { return (*tiles)[0].size(); }
        inline int32_t height() const { return (*tiles).size(); }
};

struct GRaycastResult {
        bool hit = false;
        g_float2 pos = {0.0, 0.0};
        g_float2 normal = {0., 0.};
        GEntity *hit_entity = nullptr;
};
struct GGame {
        bool should_exit = false;
        GGameState game_state = GGAME_STATE_MENU;
        GWorld world;
        GPtrSet<GEntity> entities;
        vector<GEntity *> destroy_queue;
        vector<GParticle> particles;
        GUI gui;
        // end variables
        bool check_collision_rect(GBounds b);
        GRaycastResult raycast(g_float2 start, g_float2 end,
                               const vector<GEntity *> &ignored_entities);
};

extern GGame game;

extern constexpr Vector2 to_rl(g_float2 p);
extern constexpr g_float2 to_game(Vector2 p);
GGame *get_game();
GWorld *get_world();
vector<GEntity *> get_entities();
GUI *get_gui();
void game_update();
void game_load_world(string_view path);
void game_teardown();
void menu_teardown();
void gameloop();
void menu_update();
void game_render();
void menu_render();
void player_dead_update();
void player_dead_render();
GWorld game_generate_world();
GEntity *new_entity(GEntity et);
void delete_entity(GEntity *et);
void game_setup(GEntity *previous_player);

void spawn_particle(GParticle particle);

void particle_updates(float delta_time);
void particle_rendering(float delta_time);
int32_t roll(size_t dice_count);
void player_died();
void spawn_enemy(int32_t level);
GEntity *get_player_entity();
