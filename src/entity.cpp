#include "game.h"
void GEntity::on_tick(float delta_time) {
    if (m_kind == GENTITY_KIND_PLAYER) {
        this->on_tick_player(delta_time);
    }
    else if (m_kind == GENTITY_KIND_ENEMY) {
        this->on_tick_enemy(delta_time);
    }
}

void GEntity::on_tick_player(float delta_time) {
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
        m_facing = Vector2Rotate(m_facing, -delta_time * PI);
    }
    if (IsKeyDown(KEY_E)) {
        m_facing = Vector2Rotate(m_facing, delta_time * PI);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        fire_weapon(delta_time);
    }
    move_dir = Vector2Rotate(move_dir, -Vector2Angle(m_facing, { 1., 0. }));
    this->handle_movement(delta_time, move_dir);
}

void GEntity::on_tick_enemy(float delta_time) {
    g_float2 move_dir = { 1., 0. };
    float fc = Vector2Angle(m_facing, { 1., 0. });
    float delta = (fc - m_ai_data.m_desired_facing);
    if (abs(delta) < 0.1) {
        m_facing = { cos(m_ai_data.m_desired_facing), sin(m_ai_data.m_desired_facing) };
        m_ai_data.m_desired_facing = (float(rand() % 628)) / (100.);
    }
    else if (delta > 0.) {
        m_facing = Vector2Rotate(m_facing, PI * 1.5 * delta_time);
    }
    else {
        m_facing = Vector2Rotate(m_facing, -PI * 1.5 * delta_time);
    }
    move_dir = Vector2Rotate(move_dir, -Vector2Angle(m_facing, { 1., 0. }));
    this->handle_movement(delta_time, move_dir);
}

void GEntity::on_tick_paused(float delta_time) {
    return;
}
void GEntity::on_render() {
    const double radius = (WORLD_TILE_SIZE) / 4;
    DrawCircle(m_position.x, m_position.y, radius, RED);
    g_float2 dt = Vector2Rotate(m_facing, PI / 2.);
    const double weapon_offset = 0.5;
    const double weapon_length = 1.5;
    DrawLine(m_position.x + (dt.x * radius * weapon_offset), m_position.y + dt.y * (radius * weapon_offset), m_position.x + (dt.x * radius * weapon_offset) + (m_facing.x * radius * weapon_length), m_position.y + (dt.y * radius * weapon_offset) + (m_facing.y * radius * weapon_length), BLACK);
}
GEntity* new_entity(GEntity et) {
    return get_game()->entities.create(et);
}

GBounds GEntity::get_bounds() const {
    return GBounds{ .x = int(m_position.x - m_width / 2), .y = (int)(m_position.y - m_height / 2), .width = (int)m_width, .height = (int)m_height };
}

void GEntity::handle_movement(float delta_time, g_float2 move_dir) {
    g_float2 old_pos = m_position;
    m_position += move_dir * delta_time * m_movement_speed;
    if (get_game()->check_collision_rect(this->get_bounds())) {
        m_position = old_pos;
        m_position.x += move_dir.x * delta_time * m_movement_speed / 2.;
        if (get_game()->check_collision_rect(this->get_bounds())) {
            m_position = old_pos;
            m_position.y += move_dir.y * delta_time * m_movement_speed / 2.;
            if (get_game()->check_collision_rect(this->get_bounds())) {
                m_position = old_pos;
            }
        }
    }
}

void GEntity::fire_weapon(float delta_time) {
    GRaycastResult rc = get_game()->raycast(m_position, m_position + m_facing * 3000, { this });
    if (rc.hit) {
        GParticle p = { 0 };
        p.m_color = GREEN;
        p.m_exists = true;
        p.m_pos2 = rc.pos;
        p.m_pos = m_position;
        p.m_kind = GPARTICLE_LINE;
        p.m_remaining_lifetime = delta_time;
        spawn_particle(p);
        if (rc.hit_entity) {
            rc.hit_entity->apply_damage(roll(4), G_DAMAGE_TYPE_IMPALING);
        }
    }
    else {
        GParticle p;
        p.m_color = BLUE;
        p.m_exists = true;
        p.m_pos2 = m_position + m_facing * 3000;
        p.m_pos = m_position;
        p.m_kind = GPARTICLE_LINE;
        p.m_remaining_lifetime = delta_time;
        spawn_particle(p);

    }
}
void GEntity::apply_damage(int32_t damage, GDamageType damage_type) {
    (void)damage_type;
    damage -= 8;
    if (damage < 0) {
        damage = 0;
    }
    m_health -= damage;
    printf("entity took:%d damage\n", damage);
    if (m_health < 0) {
        delete_entity(this);
    }
}