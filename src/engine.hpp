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

};

struct Wall{
    Vector2 start;
    Vector2 end; 
};

struct WorldData{
    vector<Wall> walls;
    vector<string> spawns;
};

struct EntityBox{
    unique_ptr<Entity> entity;
    uint16_t generation;
};

struct World{
    vector<Wall> walls;
    array<EntityBox, MAX_ENTITY_COUNT> entities;
};
struct Engine{

};

World* get_world();
Engine * get_engine();

struct Collision{
    bool hit_something;
    EntityRef hit_entity;
    Vector2 position;
    Vector2 normal;
};