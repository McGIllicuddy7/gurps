#include "game.h"
void GEntity::on_tick(float delta_time) {
    g_float2 move_dir = { 0.0, 0.0 };
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
        facing = Vector2Rotate(facing, -delta_time * PI);
    }
    if (IsKeyDown(KEY_E)) {
        facing = Vector2Rotate(facing, delta_time * PI);
    }
    if (IsKeyDown(KEY_SPACE)) {
        GRaycastResult rc = get_game()->raycast(position, position + facing * 3000, { this });
        if (rc.hit) {
            GParticle p = { 0 };
            p.color = GREEN;
            p.exists = true;
            p.pos2 = rc.pos;
            p.pos = position;
            p.kind = GPARTICLE_LINE;
            p.remaining_lifetime = .2;
            spawn_particle(p);

        }
        else {
            GParticle p;
            p.color = BLUE;
            p.exists = true;
            p.pos2 = position + facing * 3000;
            p.pos = position;
            p.kind = GPARTICLE_LINE;
            p.remaining_lifetime = .2;
            spawn_particle(p);

        }
    }
    move_dir = Vector2Rotate(move_dir, -Vector2Angle(facing, { 1., 0. }));
    this->handle_movement(delta_time, move_dir);
}

void GEntity::on_tick_paused(float delta_time) {
    return;
}
void GEntity::on_render() {
    DrawCircle(position.x, position.y, 10, RED);
    g_float2 dt = Vector2Rotate(facing, PI / 2.);
    DrawLine(position.x + dt.x * 5, position.y + dt.y * 5, position.x + dt.x * 5 + facing.x * 20, position.y + dt.y * 5 + facing.y * 20, BLACK);
}
GEntity* new_entity(GEntity et) {
    return get_game()->entities.create(et);
}

GBounds GEntity::get_bounds() const {
    return GBounds{ .x = int(position.x - width / 2), .y = (int)(position.y - height / 2), .width = (int)width, .height = (int)height };
}

void GEntity::handle_movement(float delta_time, g_float2 move_dir) {
    g_float2 old_pos = position;
    position += move_dir * delta_time * movement_speed;
    if (get_game()->check_collision_rect(this->get_bounds())) {
        position = old_pos;
        position.x += move_dir.x * delta_time * movement_speed / 2.;
        if (get_game()->check_collision_rect(this->get_bounds())) {
            position = old_pos;
            position.y += move_dir.y * delta_time * movement_speed / 2.;
            if (get_game()->check_collision_rect(this->get_bounds())) {
                position = old_pos;
            }
        }
    }
}