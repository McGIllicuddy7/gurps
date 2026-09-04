#include "game.h"

void GEntity::on_tick_enemy(float delta_time) {
        g_float2 move_dir = {1., 0.};
        float fc = Vector2Angle(m_facing, {1., 0.});
        float delta = (fc - m_ai_data.m_desired_facing);
        if (abs(delta) < 0.1) {
                m_facing = {cos(m_ai_data.m_desired_facing),
                            sin(m_ai_data.m_desired_facing)};
                m_ai_data.m_desired_facing = (float(rand() % 628)) / (100.);
        } else if (delta > 0.) {
                m_facing = Vector2Rotate(m_facing, PI * 1.5 * delta_time);
        } else {
                m_facing = Vector2Rotate(m_facing, -PI * 1.5 * delta_time);
        }
        GEntity *player;
        if ((player = get_player_entity())) {
                g_float2 delta_player =
                    Vector2Normalize(player->m_position - m_position);
                if (Vector2Angle(delta_player, m_facing) < 0.05) {
                        fire_weapon(delta_time);
                }
        }
        move_dir = Vector2Rotate(move_dir, -Vector2Angle(m_facing, {1., 0.}));
        this->handle_movement(delta_time, move_dir);
}
