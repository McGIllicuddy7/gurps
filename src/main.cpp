#include "utils.h"
#include "game.h"
#include <stdio.h>
#include "gui.h"

int main() {
    InitWindow(1000, 800, "gurps");
    SetTargetFPS(60);
    GUI* gui = new GUI;
    while (!WindowShouldClose()) {
        gui->begin_frame();
        gui->text("hello world!", 10, 20, 100, 20, WHITE);
        if (gui->text_button("exit", 10, 40, 100, 20, WHITE)) {
            break;
        }
        string tmp;
        if (gui->text_input(tmp, 10, 60, 100, 20, WHITE, make_id())) {
            printf("%s\n", tmp.c_str());
        }
        gui->end_frame();
        BeginDrawing();
        ClearBackground(BLACK);
        gui->render();
        EndDrawing();
    }
    CloseWindow();

}