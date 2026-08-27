#include "game.h"
Game game;
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
    game_render();
}

void game_load_world(string_view path) {

}
void game_save_world(string_view path) {}

void game_teardown() {

}

void gameloop() {
    InitWindow(1000, 800, "gurps");
    SetExitKey(0);
    while (!WindowShouldClose()) {
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
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
}

void menu_render() {
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
}