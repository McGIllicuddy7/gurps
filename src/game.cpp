
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
    int32_t player_x = WORLD_TILE_DIM / 2;
    int32_t player_y = WORLD_TILE_DIM / 2;
    int32_t x_off = 0;
    int32_t y_off = 0;
    Vector2 player_pos = { 500., 400. };
    Vector2 player_facing = Vector2{ 1., 0. };
    get_game()->entities.for_each([&](Entity& i) {
        if (i.is_player) {
            player_x = round(i.position.x / 20);
            player_y = round(i.position.y / 20);
            x_off = i.position.x - player_x * 20;
            y_off = i.position.y - player_y * 20;
            player_pos = Vector2{ .x = i.position.x + 500.f, .y = i.position.y + 400.f };
            player_facing = Vector2{ .x = i.facing.x, .y = i.facing.y };
        }
        });
    Camera2D cam = { 0 };
    cam.rotation = 0.;
    cam.zoom = 1.;
    cam.offset = player_pos;
    BeginMode2D(cam);
    for (int32_t dy = -10; dy <= 10; dy++) {
        int32_t y = player_y + dy;
        if (y < 0 || y >= WORLD_TILE_DIM) {
            continue;
        }
        for (int32_t dx = -10; dx < 10; dx++) {
            int32_t x = player_x + dx;
            if (x < 0 || x >= WORLD_TILE_DIM) {
                continue;
            }
            Color col = WHITE;
            if ((*world)[x, y].is_occupied) {
                col = BLACK;
            }
            DrawRectangle((x - WORLD_TILE_DIM / 2) * 40, (y - WORLD_TILE_DIM / 2) * 40, 40, 40, col);
        }
    }
    get_game()->entities.for_each([](Entity& i) {
        i.on_render();
        });

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