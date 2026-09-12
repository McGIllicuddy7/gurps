#pragma once
#include "utils.hpp"
#include <raylib.h>
struct TextureHandle{
    i32 id = -1;  
    int32_t get_height();
    int32_t get_width();
    Texture* get_texture();
};

struct ShaderHandle{
    i32 id;
    Shader* get_shader();
};

enum GraphicsDrawCallKind{
    DRAW_CALL_DRAW_RECTANGLE,
    DRAW_CALL_DRAW_CIRCLE, 
    DRAW_CALL_DRAW_TEXT,
    DRAW_CALL_DRAW_TRIANGLE, 
    DRAW_CALL_DRAW_PIXEL,
    DRAW_CALL_BEGIN_SCISSOR, 
    DRAW_CALL_END_SCISSOR,
    DRAW_CALL_DRAW_TEXTURE,
};

struct GraphicsDrawCall{
    GraphicsDrawCallKind kind;
    string text;
    i32 x;
    i32 y;
    i32 x1;
    i32 y1;
    i32 x2;
    i32 y2;
    i32 width;
    i32 height; 
    TextureHandle texture;
    Color color;
};

struct GraphicsQueue{
    vector<GraphicsDrawCall> commands;
};

enum GraphicsDrawCall3DKind{

};
struct GraphicsDrawCall3D{

};
struct GraphicsRuntime{ 
    vector<Texture2d> textures;
    vector<Shader> shaders;
    vector<GraphicsQueue> graphics_queues_2d;
};

extern GraphicsRuntime graphics_runtime;
void graphics_runtime_bind_graphics_queue(const GraphicsQueue & queue);
void graphics_runtime_update();

GraphicsQueue new_graphics_queue();