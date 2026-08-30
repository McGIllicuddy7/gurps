
#include "game.h"
#include <unordered_set>
GGame game;
void game_tick();
extern constexpr Vector2 to_rl(g_float2 p) {
    return { p.x, p.y };
}

extern constexpr g_float2 to_game(Vector2 p) {
    return { p.x, p.y };
}

GWorld* get_world() {
    return &game.world;
}

vector<GEntity*> get_entities() {
    return game.entities.values();
}

void game_update() {
    game_tick();
    game_render();
}

void game_load_world(string_view path) {

}

void game_save_world(string_view path) {

}

void game_teardown() {

}

void gameloop() {
    InitWindow(1000, 800, "gurps");
    //  SetTargetFPS(61);
    SetExitKey(0);
    while (!WindowShouldClose()) {
        get_game()->should_exit = false;
        [[likely]]
        if (get_game()->is_running) {
            game_update();
            if (get_game()->should_exit) {
                game_teardown();
                get_game()->should_exit = false;
                get_game()->is_running = false;
            }
        }
        else {
            menu_update();
            if (get_game()->should_exit) {
                menu_teardown();
                break;
            }
        }
    }
}

void menu_update() {
    menu_render();
}

GGame* get_game() {
    return &game;
}

void menu_teardown() {


}

void game_render() {
    get_gui()->begin_frame();
    BeginDrawing();
    ClearBackground(BLACK);
    GWorld* world = get_world();
    int32_t player_x = 0;
    int32_t player_y = 0;
    int32_t x_off = 0;
    int32_t y_off = 0;
    Vector2 player_pos = { 500., 400. };
    Vector2 player_facing = Vector2{ 1., 0. };
    get_game()->entities.for_each([&](GEntity& i) {
        if (i.is_player) {
            player_x = round(i.position.x / WORLD_TILE_SIZE);
            player_y = round(i.position.y / WORLD_TILE_SIZE);
            x_off = i.position.x - player_x * WORLD_TILE_SIZE;
            y_off = i.position.y - player_y * WORLD_TILE_SIZE;
            player_pos = Vector2{ .x = -i.position.x + 500.f, .y = -i.position.y + 400.f };
            player_facing = Vector2{ .x = i.facing.x, .y = i.facing.y };
        }
        });
    Camera2D cam = { 0 };
    cam.rotation = 0.;
    cam.zoom = 1.;
    cam.offset = player_pos;
    BeginMode2D(cam);
    int32_t delta = 8;
    int32_t tile_size_px = WORLD_TILE_SIZE;
    BeginScissorMode(500 - delta * tile_size_px, 400 - delta * tile_size_px, delta * 2 * tile_size_px, delta * 2 * tile_size_px);
    for (int32_t dy = -delta - 1; dy <= delta + 1; dy++) {
        int32_t y = player_y + dy;
        if (y < 0 || y >= WORLD_TILE_DIM) {
            continue;
        }
        for (int32_t dx = -delta - 1; dx <= delta + 1; dx++) {
            int32_t x = player_x + dx;
            if (x < 0 || x >= WORLD_TILE_DIM) {
                continue;
            }
            Color col = WHITE;
            if ((*world)[x, y].is_occupied) {
                col = BLACK;
            }
            DrawRectangle((x)*tile_size_px - tile_size_px / 2, (y)*tile_size_px - tile_size_px / 2, tile_size_px, tile_size_px, col);
        }
    }
    get_game()->entities.for_each([](GEntity& i) {
        i.on_render();
        });
    particle_rendering(GetFrameTime());
    EndScissorMode();

    EndMode2D();
    /*
    get_gui()->begin_panel(100, 100, 600, 400);
    get_gui()->text("GAME RUNNING", 40, WHITE);
    if (get_gui()->text_button("exit", 20, WHITE)) {
        game.is_running = false;
        game.should_exit = true;
    }
    get_gui()->end_panel();
    */
    get_gui()->end_frame();
    get_gui()->render();
    DrawFPS(10, 10);
    EndDrawing();
}

void menu_render() {
    BeginDrawing();
    ClearBackground(BLACK);
    get_gui()->begin_frame();
    get_gui()->begin_panel(100, 100, 600, 400);
    if (get_gui()->text_button("play game", 40, WHITE)) {
        game.is_running = true;
        get_game()->world = game_generate_world();
        game_setup(nullptr);
    }
    if (get_gui()->text_button("exit", 30, WHITE)) {
        game.should_exit = true;
    }
    get_gui()->end_panel();
    get_gui()->end_frame();
    get_gui()->render();
    EndDrawing();
}

GUI* get_gui() {
    return &game.gui;
}

void game_tick() {
    float delta_time = GetFrameTime();
    if (!get_game()->is_paused) {
        get_game()->entities.for_each([=](GEntity& i) {
            i.on_tick(delta_time);
            });
        particle_updates(delta_time);
    }
    else {
        get_game()->entities.for_each([=](GEntity& i) {
            i.on_tick_paused(delta_time);
            });
    }
}

void game_setup(GEntity* previous_player) {
    game.particles.clear();
    if (previous_player) {
        GEntity player = *previous_player;
        get_game()->entities.clear();
        new_entity(player);
    }
    else {
        GEntity player = { };
        player.kind = GENTITY_KIND_PLAYER;
        player.is_player = true;
        player.position = { 0., 0. };
        player.facing = { 1., 0. };
        player.health = 10;
        player.height = 16;
        player.width = 16;
        new_entity(player);
    }
}

bool GGame::check_collision_rect(GBounds b) {
    int32_t base_x = b.x / WORLD_TILE_SIZE;
    int32_t base_y = b.y / WORLD_TILE_SIZE;
    for (int32_t dy = -1; dy <= 1; dy++) {
        for (int32_t dx = -1; dx <= 1; dx++) {
            GBounds b2 = GBounds{ .x = (base_x + dx) * WORLD_TILE_SIZE - WORLD_TILE_SIZE / 2, .y = (base_y + dy) * WORLD_TILE_SIZE - WORLD_TILE_SIZE / 2, .width = WORLD_TILE_SIZE,.height = WORLD_TILE_SIZE };
            Rectangle r1 = { .x = float(b.x), .y = float(b.y), .width = float(b.width), .height = float(b.height) };
            Rectangle r2 = { .x = float(b2.x), .y = float(b2.y), .width = float(b2.width), .height = float(b2.height) };
            if (CheckCollisionRecs(r1, r2)) {
                int32_t bx = base_x + dx;
                int32_t by = base_y + dy;
                if (bx < 0 || bx >= WORLD_TILE_DIM || by < 0 || by >= WORLD_TILE_DIM) {
                    return true;
                }
                if ((*get_world())[bx, by].is_occupied) {
                    return true;
                }
            }
        }
    }
    return false;
}
bool check_raycast_rectangle(g_float2 start, g_float2 end, Rectangle r, g_float2* output, g_float2* normal) {
    Vector2 sv = { start.x, start.y };
    Vector2 ev = { end.x, end.y };
    if (CheckCollisionPointRec(sv, r)) {
        if (output) {
            *output = start;
        }
        if (normal) {
            *normal = Vector2Normalize(ev - sv);
        }
        return true;
    }
    Vector2 r0 = { r.x, r.y };
    Vector2 r1 = { r.x + r.width, r.y };
    Vector2 r2 = { r.x , r.y + r.height };
    Vector2 r3 = { r.x + r.width, r.y + r.height };
    Vector2 tmp;
    g_float2 nearest_point = end;
    float nearest_distance = Vector2Length(end - start);
    bool hit = false;
    g_float2 nearest_normal = { 1., 0.0 };
    if (CheckCollisionLines(sv, ev, r0, r1, &tmp)) {
        g_float2 t = { tmp.x, tmp.y };
        float dist = Vector2Length(t - start);
        if (dist < nearest_distance) {
            nearest_point = t;
            nearest_distance = dist;
            hit = true;
            nearest_normal = { -1., 0. };
        }
    }
    if (CheckCollisionLines(sv, ev, r0, r2, &tmp)) {
        g_float2 t = { tmp.x, tmp.y };
        float dist = Vector2Length(t - start);
        if (dist < nearest_distance) {
            nearest_point = t;
            nearest_distance = dist;
            hit = true;
            nearest_normal = { 0., -1. };
        }
    }
    if (CheckCollisionLines(sv, ev, r1, r3, &tmp)) {
        g_float2 t = { tmp.x, tmp.y };
        float dist = Vector2Length(t - start);
        if (dist < nearest_distance) {
            nearest_point = t;
            nearest_distance = dist;
            hit = true;
            nearest_normal = { 0., 1. };
        }
    }
    if (CheckCollisionLines(sv, ev, r2, r3, &tmp)) {
        g_float2 t = { tmp.x, tmp.y };
        float dist = Vector2Length(t - start);
        if (dist < nearest_distance) {
            nearest_point = t;
            nearest_distance = dist;
            hit = true;
            nearest_normal = { 1., 0. };
        }
    }
    if (hit) {
        if (output) {
            *output = nearest_point;
        }
        if (normal) {
            *normal = nearest_normal;
        }
    }
    return hit;
}

GRaycastResult GGame::raycast(g_float2 start, g_float2 end, const vector<GEntity*>& ignored_entities) {
    if (end == start) {
        return {};
    }
    g_float2 closest = start;
    float closest_dist = Vector2Length(end - start);
    g_float2 closest_normal = { 1., 0.0 };
    bool hit_something = false;
    GEntity* hit_entity = nullptr;
    vector<g_int2> queue;
    queue.reserve(8 * (Vector2Length(end - start) / WORLD_TILE_SIZE));
    queue.push_back({ (int)round(start.x / (WORLD_TILE_SIZE)), (int)round(start.y / (WORLD_TILE_SIZE)) });
    std::unordered_set<g_int2> reached;
    reached.reserve(8 * (Vector2Length(end - start) / WORLD_TILE_SIZE));
    while (!queue.empty()) {
        g_int2 p = queue[queue.size() - 1];
        queue.pop_back();
        int32_t x = p.x;
        int32_t y = p.y;
        for (int32_t dy = -1; dy <= 1; dy++) {
            for (int32_t dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) {
                    continue;
                }
                g_int2 p2 = { x + dx, y + dy };
                if (reached.contains(p2)) {
                    continue;
                }
                if (p2.x < 0 || p2.y < 0 || p2.x >= WORLD_TILE_DIM || p2.y >= WORLD_TILE_DIM) {
                    continue;
                }
                g_float2 pos = { float(p2.x) * WORLD_TILE_SIZE, float(p2.y) * WORLD_TILE_SIZE };
                Rectangle r = Rectangle{ .x = pos.x - WORLD_TILE_SIZE / 2 , .y = pos.y - WORLD_TILE_SIZE / 2,.width = WORLD_TILE_SIZE , .height = WORLD_TILE_SIZE };
                g_float2 tpos;
                g_float2 tnorm;
                if (check_raycast_rectangle(start, end, r, &tpos, &tnorm)) {
                    float len = Vector2Length((tpos - start));
                    queue.push_back(p2);
                    reached.insert(p2);
                    if (len < closest_dist && this->world[p2.x, p2.y].is_occupied) {
                        closest = tpos;
                        closest_normal = tnorm;
                        hit_something = true;
                        closest_dist = len;
                    }
                }
            }
        }
    }
    entities.for_each([&](GEntity& i) {
        for (const auto& j : ignored_entities) {
            if (&i == j) {
                return;
            }
        }
        GBounds b = i.get_bounds();
        Rectangle r = { float(b.x), float(b.y), float(b.width), float(b.height) };
        g_float2 tpos;
        g_float2 tnorm;
        if (check_raycast_rectangle(start, end, r, &tpos, &tnorm)) {
            float len = Vector2Length(tpos - start);
            if (len < closest_dist) {
                closest = tpos;
                closest_normal = tnorm;
                hit_something = true;
                closest_dist = len;
                hit_entity = &i;
            }
        }});
        GRaycastResult out = {};
        if (hit_something) {
            out.hit = true;
            out.normal = closest_normal;
            out.hit_entity = hit_entity;
            out.pos = closest;
        }
        return out;
}

void particle_updates(float delta_time) {
    for (GParticle& i : game.particles) {
        if (i.exists) {
            i.pos += i.velocity * delta_time;
            i.remaining_lifetime -= delta_time;
            if (i.remaining_lifetime < 0.) {
                i.exists = false;
            }
        }
    }
}

void particle_rendering(float delta_time) {
    (void)delta_time;
    for (const GParticle& i : game.particles) {
        if (i.exists) {
            switch (i.kind) {
            case GPARTICLE_LINE: {
                DrawLine(i.pos.x, i.pos.y, i.pos2.x, i.pos2.y, i.color);
            }
            case GPARTICLE_PARTICLE: {
                DrawCircle(i.pos.x, i.pos.y, i.radius, i.color);
            }
            case GPARTICLE_SQUARE: {
                DrawRectanglePro(Rectangle{ i.pos.x - i.pos.x / 2, i.pos.y - i.height / 2 }, { 0., 0.0 }, i.rotation, i.color);
            }
            }
        }
    }
}

void spawn_particle(GParticle particle) {
    for (GParticle& i : game.particles) {
        if (!i.exists) {
            i = particle;
            return;
        }
    }
    game.particles.push_back(particle);
}