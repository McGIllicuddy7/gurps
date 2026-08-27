#pragma once
#include "utils.h"
#include <raylib.h>
extern uint32_t frame_counter;
#define make_id(...)  _generate_id(__LINE__, __FILE__ __VA_OPT__(,)__VA_ARGS__)
inline std::string _generate_id(int32_t line, const char* file, size_t idx) {
    string out;
    char buff[256];
    snprintf(buff, 255, "%zu", idx);
    out += file;
    out += "-";
    out += buff;
    snprintf(buff, 255, "%d", line);
    out += "-";
    out += buff;


    return out;
}
inline std::string _generate_id(int32_t line, const char* file) {
    string out;
    char buff[256];
    out += file;
    snprintf(buff, 255, "%d", line);
    out += "-";
    out += buff;
    return out;
}
inline std::string _generate_id(int32_t line, const char* file, const std::string& s) {
    string out;
    char buff[256];
    out += file;
    snprintf(buff, 255, "%d", line);
    out += "-";
    out += buff;
    out += "-";
    out += s;
    return out;
}
enum DrawCallKind {
    DRAW_CALL_NONE,
    DRAW_CALL_DRAW_TEXT,
    DRAW_CALL_DRAW_RECTANGLE,
    DRAW_CALL_DRAW_RECTANGLE_LINES,
    DRAW_CALL_DRAW_CIRCLE,
    DRAW_CALL_DRAW_IMAGE,
    DRAW_CALL_DRAW_TEXTURE,
    DRAW_CALL_BEGIN_SCISSOR,
    DRAW_CALL_END_SCISSOR,
};

struct DrawCall {
    DrawCallKind kind = DRAW_CALL_NONE;
    std::string text = "";
    int32_t x = 0;
    int32_t y = 0;
    int32_t width = 0;
    int32_t height = 0;
    Color color = BLACK;
    int32_t text_height = 0;
    Texture2D texture = { 0 };
    static DrawCall draw_text(string_view str, int32_t x, int32_t y, int32_t text_height, Color color);
    static DrawCall draw_rectangle(int32_t x, int32_t y, int32_t width, int32_t height, Color color);
    static DrawCall draw_rectangle_lines(int32_t x, int32_t y, int32_t width, int32_t height, Color color);
    static DrawCall draw_circle(int32_t x, int32_t y, int32_t radius, Color color);
    static DrawCall draw_image(string_view str, int32_t x, int32_t y, int32_t width, int32_t height, Color color);
    static DrawCall draw_texture(Texture2D texture, int32_t x, int32_t y, int32_t width, int32_t height, Color color);
    static DrawCall begin_scissor(int32_t x, int32_t y, int32_t width, int32_t height);
    static DrawCall end_scissor();
};

struct Panel {
    int32_t x, y, width, height, offset = 0;
};

class GUI {
    std::string m_current_selected_object;
    std::unordered_map<string, int32_t> m_scroll_box_scroll_amounts;
    std::unordered_map<string, Texture2D> m_textures;
    std::unordered_map<string, string> m_text_inputs;
    vector<DrawCall> m_draw_calls;
    vector<Panel> m_panel_stack;
    Panel m_current_panel;
public:
    bool text_button(string_view text, int32_t x, int32_t y, int32_t width, int32_t height, Color color);

    void text(string_view text, int32_t x, int32_t y, int32_t width, int32_t height, Color color);
    bool text_input(string& output, int32_t x, int32_t y, int32_t width, int32_t height, Color color, const string& id);

    void text_scroll_box(arr<string_view> text, int32_t x, int32_t y, int32_t width, int32_t height, int32_t text_height, Color color, const string& id);

    int32_t text_button_scroll_box(arr<string_view> text, int32_t x, int32_t y, int32_t width, int32_t height, int32_t text_height, Color color, const string& id);

    void image(string_view image, int32_t x, int32_t y, int32_t width, int32_t height, Color color);

    void reset();
    void begin_frame();
    void end_frame();
    void render();
    void begin_panel(int32_t x, int32_t y, int32_t width, int32_t height);
    void end_panel();
    void draw_text(string_view str, int32_t x, int32_t y, int32_t text_height, Color color);
    void draw_rectangle(int32_t x, int32_t y, int32_t width, int32_t height, Color color);
    void draw_rectangle_lines(int32_t x, int32_t y, int32_t width, int32_t height, Color color);
    void draw_circle(int32_t x, int32_t y, int32_t radius, Color color);
    void draw_image(string_view img, int32_t x, int32_t y, int32_t width, int32_t height, Color color);
    void draw_texture(Texture2D texture, int32_t x, int32_t y, int32_t width, int32_t height, Color color);
    void begin_scissor(int32_t x, int32_t y, int32_t width, int32_t height);
    void end_scissor();
};

