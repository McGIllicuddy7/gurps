#include "game.h"
World game_generate_world() {
    World out;
    out.tiles = make_unique<array<array<Tile, WORLD_TILE_DIM>, WORLD_TILE_DIM>>();
    for (size_t y = 0; y < WORLD_TILE_DIM; y++) {
        for (size_t x = 0; x < WORLD_TILE_DIM; x++) {
            out[x, y].is_occupied = rand() % 100 < 20;
        }
    }
    return out;
}