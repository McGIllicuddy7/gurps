#include "utils.h"
#include "game.h"
#include <stdio.h>
#include "gui.h"

int main() {
    InitWindow(1000, 800, "gurps");
    SetTargetFPS(60);
    GUI* gui = new GUI;
    vector<string > strings;
    for (int32_t i = 0; i < 10; i++) {
        char buff[100];
        snprintf(buff, 99, "%d", i);
        strings.push_back(string(buff));
    }
    vector<string_view> lorem_ips = {};
    for (int i = 0; i < 10; i++) {
        lorem_ips.push_back(LOREM_IPSUM);
    }
    lorem_ips.push_back("end");
    while (!WindowShouldClose()) {
        gui->begin_frame();
        gui->begin_panel(20, 20, 640, 480);
        gui->text("hello world!", 20, WHITE);
        if (gui->text_button("exit", 20, WHITE)) {
            break;
        }
        string tmp;
        if (gui->text_input(tmp, 20, WHITE, make_id())) {
            strings.push_back(tmp);
        }
        {
            vector<string_view> strings2;
            for (auto& i : strings) {
                strings2.push_back(i);
            }
            int32_t idx = gui->text_button_scroll_box(strings2, 400, 20, WHITE, make_id());
            if (idx != -1) {
                strings.erase(strings.begin() + idx);
            }

        }
        gui->text_scroll_box(lorem_ips, 300, 10, WHITE, make_id());
        gui->end_panel();
        gui->end_frame();
        BeginDrawing();
        ClearBackground(BLACK);
        gui->render();
        EndDrawing();
    }
    CloseWindow();

}