#include "engine.hpp"
static Engine game_engine;

World* get_world(){
    return &game_engine.world;
}
Engine * get_engine(){
    return &game_engine;
}
void game_loop(){
    game_runtime_setup();
    game_engine_init();
    while(!WindowShouldClose()){
        switch (get_engine()->game_state){
            case GAME_STATE_PAUSED:{
                game_iteration_paused();
                break;
            }
            case GAME_STATE_MENU:{
                game_iteration_menu();
                break;
            }
            case GAME_STATE_PLAYING:{
                game_iteration_playing();
                break;
            }
            default:
            assert(false);
        }
    }
}

void game_engine_init(){
    InitWindow(640*2, 480*2, "murderbot");
    SetExitKey(0);
    InitAudioDevice();
    game_engine.game_state = GAME_STATE_MENU;
}

void game_iteration_paused(){
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("paused", 640,200, 48, WHITE);
    if(IsKeyPressed(KEY_SPACE)){
        get_engine()->game_state = GAME_STATE_PLAYING;
    }
    EndDrawing();
}
void game_iteration_menu(){
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("press any key to start", 640, 200, 48, WHITE);
    if(GetCharPressed()){
        get_engine()->game_state = GAME_STATE_PLAYING;
        game_load();
    }
    EndDrawing();
}

void game_iteration_playing(){
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("press escape to pause", 640, 200, 48, WHITE);
    if(IsKeyPressed(KEY_ESCAPE)){
        game_engine.game_state = GAME_STATE_PAUSED;
    }
    EndDrawing();
}

void game_load(){}