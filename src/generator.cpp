#include "game.h"
GWorld game_generate_world() {
        GWorld out;
        out.tiles =
            unique_ptr<array<array<GTile, WORLD_TILE_DIM>, WORLD_TILE_DIM>>{
                new array<array<GTile, WORLD_TILE_DIM>, WORLD_TILE_DIM>()};
        for (size_t y = 0; y < WORLD_TILE_DIM; y++) {
                for (size_t x = 0; x < WORLD_TILE_DIM; x++) {
                        out(x, y).is_occupied = rand() % 100 < 10;
                }
        }
        return out;
}


vector<g_int2> pathfind(GWorld& world,g_int2 from, g_int2 to){	
	vector<g_int2> open_set = {from};
	todo();
}
