#+private
package main
import "vendor:raylib"
import "base:runtime"
import "core:strings"
rl :: raylib
Vector3 :: rl.Vector3
Vector2 ::rl.Vector2
Quaternion ::rl.Quaternion
Transform ::rl.Transform
MAX_ENTITY_COUNT :: 16384
EntityKind ::enum {

}

Entity::struct {
    name:string,
    kind :EntityKind,
    is_player:bool,
    transform :Transform 
}

EntitySlot ::struct {
    ent:Maybe(Entity),
    generation:u32
}

WorldObject ::struct {
    transform:Transform,
}
World:: struct {
    entities: [MAX_ENTITY_COUNT] Entity, 
    static_world_objects:[dynamic] WorldObject,
    loaded_meshes:map[string]rl.Model,
    level_arena:runtime.Arena,
    frame_arena:runtime.Arena
}
Runtime :: struct {
    world : World
}

@(private)
game_runtime:Runtime

level_allocator:: proc()->runtime.Allocator{
    return runtime.arena_allocator(&game_runtime.world.level_arena)
}

frame_allocator ::proc()->runtime.Allocator{
    return runtime.arena_allocator(&game_runtime.world.frame_arena)
}

general_allocator:: proc()->runtime.Allocator{
    return context.allocator
}
