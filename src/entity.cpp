#include "game.h"
void Entity::on_tick(float delta_time) {
    float2 move_dir = { 0.0, 0.0 };
    if (IsKeyDown(KEY_W)) {
        move_dir.x += 1.;
    }
    if (IsKeyDown(KEY_S)) {
        move_dir.x -= 1.;
    }
    if (IsKeyDown(KEY_A)) {
        move_dir.y -= 1.;
    }
    if (IsKeyDown(KEY_D)) {
        move_dir.y += 1.;
    }
    if (IsKeyDown(KEY_Q)) {
        facing = facing.rotated_by(-delta_time * 1.5 * PI);
    }
    if (IsKeyDown(KEY_E)) {
        facing = facing.rotated_by(delta_time * 1.5 * PI);
    }
    move_dir = move_dir.rotated_by(facing.angle());
    float2 old_pos = position;
    position = position + move_dir * delta_time * 80.;
    if (get_game()->check_collision_rect(this->get_bounds())) {
        position = old_pos;
    }
}

void Entity::on_tick_paused(float delta_time) {
    return;
}
void Entity::on_render() {
    DrawCircle(position.x, position.y, 10, RED);
    float2 dt = facing.rotated_by(PI / 2.);
    DrawLine(position.x + dt.x * 5, position.y + dt.y * 5, position.x + dt.x * 5 + facing.x * 20, position.y + dt.y * 5 + facing.y * 20, BLACK);
}
Entity* new_entity(Entity et) {
    return get_game()->entities.create(et);
}

Bounds Entity::get_bounds() const {
    return Bounds{ .x = int(position.x - width / 2), .y = (int)(position.y - height / 2), .width = (int)width, .height = (int)height };
}