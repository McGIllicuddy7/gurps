#pragma once
#include <cstdint>
#include "utils.h"
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
    string name;
    float2 position;
    float2 velocity;
    int32_t health;
    int32_t ammo[WEAPON_KIND_COUNT];
};

struct Line {
    float2 start;
    float2 end;
};

struct World {
    vector<Line> walls;
};
struct Game {
    World world;

};