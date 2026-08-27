#include "utils.h"
#include "game.h"
#include <stdio.h>
#include "gui.h"

int main() {
    InitWindow(1000, 800, "gurps");
    SetTargetFPS(60);
    GUI* gui = new GUI;
    vector<string > strings;
    for (int32_t i = 0; i < 1000; i++) {
        char buff[100];
        snprintf(buff, 99, "%d", i);
        strings.push_back(string(buff));
    }
    while (!WindowShouldClose()) {
        gui->begin_frame();
        gui->text("hello world!", 10, 20, 100, 20, WHITE);
        if (gui->text_button("exit", 10, 40, 100, 20, WHITE)) {
            break;
        }
        string tmp;
        if (gui->text_input(tmp, 10, 60, 100, 20, WHITE, make_id())) {
            strings.push_back(tmp);
        }
        {
            vector<string_view> strings2;
            for (auto& i : strings) {
                strings2.push_back(i);
            }
            int32_t idx = gui->text_button_scroll_box(strings2, 10, 100, 100, 200, 20, WHITE, make_id());
            if (idx != -1) {
                strings.erase(strings.begin() + idx);
            }
        }
        gui->end_frame();
        BeginDrawing();
        ClearBackground(BLACK);
        gui->render();
        EndDrawing();
    }
    CloseWindow();

}