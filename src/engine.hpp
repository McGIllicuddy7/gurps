#pragma once
#include <raylib.h>
#include <raymath.h>
#include "utils.hpp"

constexpr size_t MAX_ENTITY_COUNT = 16000;
class Entity;

class EntityRef{
    uint16_t idx;
    uint16_t generation;
    public:
    bool is_valid() const;
    Entity* operator->();
    const Entity* operator->()const;
    Entity* operator*();
    const Entity* operator*() const;
    const Entity* get() const;
    Entity* get();
};

class Entity{
    Vector2 m_position;
    Vector2 m_velocity;
    float m_width;
    float m_height;
    float m_rotation;
    public:
    virtual ~Entity();
    virtual void on_tick(float delta_time);
    virtual void on_collision(EntityRef collider);
    virtual void on_interact(EntityRef interactor);
    virtual void on_damage(EntityRef source, float damage);
};

struct EntityClass{
    string name; 
    Entity* (*spawn)(Vector2, float);
    Entity* (*deserialize)(BiteStream& stream);
    void (*serialize)(Entity* et, BiteStream &stream);
};

struct Spawn{
    string spawned_class;
    Vector2 position;
};

struct Wall{
    Vector2 start;
    Vector2 end;   
    int32_t height; 
};

struct WorldData{
    vector<Wall> walls;
    vector<Spawn> spawns;
};

struct EntityBox{
    unique_ptr<Entity> entity = unique_ptr<Entity>(nullptr);
    uint16_t generation =0;
};

struct World{
    array<EntityBox, MAX_ENTITY_COUNT> entities;
};

enum GameState{
    GAME_STATE_PAUSED, 
    GAME_STATE_MENU, 
    GAME_STATE_PLAYING,
};

struct Engine{ 
    World world;
    GameState game_state;
    vector<EntityClass> class_registery;
};

World* get_world();
Engine * get_engine();

struct Collision{
    bool hit_something;
    EntityRef hit_entity;
    Vector2 position;
    Vector2 normal;
};


void game_iteration_paused();
void game_iteration_menu();
void game_iteration_playing();
void game_loop();
void game_engine_init();
extern void game_runtime_setup();
void game_load();