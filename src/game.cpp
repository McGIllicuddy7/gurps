
#include "game.h"

Game game;
void game_tick();
extern constexpr Vector2 to_rl(float2 p) {
    return { p.x, p.y };
}
extern constexpr float2 to_game(Vector2 p) {
    return { p.x, p.y };
}
World* get_world() {
    return &game.world;
}
vector<Entity*> get_entities() {
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
    SetTargetFPS(61);
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

Game* get_game() {
    return &game;
}
void menu_teardown() {


}
void game_render() {
    get_gui()->begin_frame();
    BeginDrawing();
    ClearBackground(BLACK);
    World* world = get_world();
    int32_t player_x = 0;
    int32_t player_y = 0;
    int32_t x_off = 0;
    int32_t y_off = 0;
    Vector2 player_pos = { 500., 400. };
    Vector2 player_facing = Vector2{ 1., 0. };
    get_game()->entities.for_each([&](Entity& i) {
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
    get_game()->entities.for_each([](Entity& i) {
        i.on_render();
        });
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
        get_game()->entities.for_each([=](Entity& i) {
            i.on_tick(delta_time);
            });
    }
    else {
        get_game()->entities.for_each([=](Entity& i) {
            i.on_tick_paused(delta_time);
            });
    }
}

void game_setup(Entity* previous_player) {
    if (previous_player) {
        Entity player = *previous_player;
        get_game()->entities.clear();
        new_entity(player);
    }
    else {
        Entity player = { };
        player.kind = ENTITY_KIND_PLAYER;
        player.is_player = true;
        player.position = { 0., 0. };
        player.facing = { 1., 0. };
        player.health = 10;
        player.height = 20;
        player.width = 20;
        new_entity(player);
    }
}

bool Game::check_collision_rect(Bounds b) {
    int32_t base_x = b.x / WORLD_TILE_SIZE;
    int32_t base_y = b.y / WORLD_TILE_SIZE;
    for (int32_t dy = -1; dy <= 1; dy++) {
        for (int32_t dx = -1; dx <= 1; dx++) {
            Bounds b2 = Bounds{ .x = (base_x + dx) * WORLD_TILE_SIZE - WORLD_TILE_SIZE / 2, .y = (base_y + dy) * WORLD_TILE_SIZE - WORLD_TILE_SIZE / 2, .width = WORLD_TILE_SIZE,.height = WORLD_TILE_SIZE };
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
bool check_raycast_rectangle(float2 start, float2 end, Rectangle r, float2* output, float2* normal) {
    Vector2 sv = { start.x, start.y };
    Vector2 ev = { end.x, end.y };
    if (CheckCollisionPointRec(sv, r)) {
        if (output) {
            *output = start;
        }
        if (normal) {
            *normal = (end - start) / (end - start).len();
        }
        return true;
    }
    Vector2 r0 = { r.x, r.y };
    Vector2 r1 = { r.x + r.width, r.y };
    Vector2 r2 = { r.x , r.y + r.height };
    Vector2 r3 = { r.x + r.width, r.y + r.height };
    Vector2 tmp;
    float2 nearest_point = end;
    float nearest_distance = (end - start).len();
    bool hit = false;
    float2 nearest_normal = { 1., 0.0 };
    if (CheckCollisionLines(sv, ev, r0, r1, &tmp)) {
        float2 t = { tmp.x, tmp.y };
        float dist = (t - start).len();
        if (dist < nearest_distance) {
            nearest_point = t;
            nearest_distance = dist;
            hit = true;
            nearest_normal = { -1., 0. };
        }
    }
    if (CheckCollisionLines(sv, ev, r0, r2, &tmp)) {
        float2 t = { tmp.x, tmp.y };
        float dist = (t - start).len();
        if (dist < nearest_distance) {
            nearest_point = t;
            nearest_distance = dist;
            hit = true;
            nearest_normal = { 0., -1. };
        }
    }
    if (CheckCollisionLines(sv, ev, r1, r3, &tmp)) {
        float2 t = { tmp.x, tmp.y };
        float dist = (t - start).len();
        if (dist < nearest_distance) {
            nearest_point = t;
            nearest_distance = dist;
            hit = true;
            nearest_normal = { 0., 1. };
        }
    }
    if (CheckCollisionLines(sv, ev, r2, r3, &tmp)) {
        float2 t = { tmp.x, tmp.y };
        float dist = (t - start).len();
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

RaycastResult Game::raycast(float2 start, float2 end) {
    if (end == start) {
        return {};
    }
    float2 closest = start;
    float closest_dist = (end - start).len();
    float2 closest_normal = { 1., 0.0 };
    bool hit_something = false;
    Entity* hit_entity = nullptr;
    for (int32_t y = 0; y < WORLD_TILE_DIM; y++) {
        for (int32_t x = 0; x < WORLD_TILE_DIM; x++) {
            if ((*get_world())[x, y].is_occupied) {
                float2 pos = { float(x) * WORLD_TILE_SIZE, float(y) * WORLD_TILE_SIZE };
                Rectangle r = Rectangle{ .x = pos.x - WORLD_TILE_SIZE / 2, .y = pos.y - WORLD_TILE_SIZE / 2, .width = WORLD_TILE_SIZE, .height = WORLD_TILE_SIZE };
                float2 tpos;
                float2 tnorm;
                if (check_raycast_rectangle(start, end, r, &tpos, &tnorm)) {
                    float len = (tpos - start).len();
                    if (len < closest_dist) {
                        closest = tpos;
                        closest_normal = tnorm;
                        hit_something = true;
                        closest_dist = len;
                    }
                }
            }
        }
    }
    entities.for_each([&](Entity& i) {
        Bounds b = i.get_bounds();
        Rectangle r = { float(b.x), float(b.y), float(b.width), float(b.height) };
        float2 tpos;
        float2 tnorm;
        if (check_raycast_rectangle(start, end, r, &tpos, &tnorm)) {
            float len = (tpos - start).len();
            if (len < closest_dist) {
                closest = tpos;
                closest_normal = tnorm;
                hit_something = true;
                closest_dist = len;
                hit_entity = &i;
            }
        }});
        RaycastResult out = {};
        if (hit_something) {
            out.hit = true;
            out.normal = closest_normal;
            out.hit_entity = hit_entity;
            out.pos = closest;
        }
        return out;
}
