#include "gui.h"
Color feature_color(Color base);
Color background_color(Color base);
Color border_color(Color base);
Color selected_color(Color base);
Color hovered_color(Color base);
uint32_t frame_counter;
bool GUI::text_button(string_view text, int32_t x, int32_t y, int32_t width, int32_t height, Color color) {
    string txt = string(text);
    int32_t text_height = (height * 4) / 5;
    int32_t y_offset = height / 10;
    int32_t t_width = MeasureText(txt.c_str(), text_height);
    assert(t_width <= width);
    int32_t x_offset = (width - t_width) / 2;
    x += m_current_panel.x;
    y += m_current_panel.y;
    bool col = CheckCollisionPointRec(GetMousePosition(), { float(x), float(y), float(width), float(height) });
    bool hit = col && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    if (hit) {
        color = selected_color(color);
    }
    else if (col) {
        color = hovered_color(color);
    }
    Color text_color = feature_color(color);
    Color bg_color = background_color(color);
    Color bounds_color = border_color(color);
    draw_rectangle(x, y, width, height, bg_color);
    draw_rectangle_lines(x, y, width, height, bounds_color);
    draw_text(text, x + x_offset, y + y_offset, text_height, text_color);
    m_current_panel.offset += height + 2;
    return col && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}

void GUI::text(string_view text, int32_t x, int32_t y, int32_t width, int32_t height, Color color) {
    string txt = string(text);
    int32_t text_height = (height * 4) / 5;
    int32_t y_offset = height / 10;
    int32_t t_width = MeasureText(txt.c_str(), text_height);
    assert(t_width <= width);
    int32_t x_offset = (width - t_width) / 2;
    x += m_current_panel.x;
    y += m_current_panel.y;
    Color text_color = feature_color(color);
    draw_text(text, x, y, text_height, color);
    m_current_panel.offset += height + 2;
}

bool GUI::text_input(string& output, int32_t x, int32_t y, int32_t width, int32_t height, Color color, const string& id) {
    if (!m_text_inputs.contains(id)) {
        m_text_inputs[id] = "";
    }
    string& txt = m_text_inputs[id];
    int32_t text_height = (height * 4) / 5;
    x += m_current_panel.x;
    y += m_current_panel.y;
    int32_t y_offset = height / 10;
    int32_t t_width = MeasureText(txt.c_str(), text_height);
    int32_t x_offset = 0;
    bool selected = false;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ float(x), float(y), float(width), float(height) })) {
            m_current_selected_object = id;
            selected = true;
        }
        else {
            if (m_current_selected_object == id) {
                m_current_selected_object = "";
            }
        }
    }
    else {
        if (id == m_current_selected_object) {
            selected = true;
        }
    }
    bool to_return = false;
    if (selected) {
        int32_t c = GetCharPressed();
        if (c) {
            txt.push_back(c);
        }
        if (IsKeyPressed(KEY_DELETE) || IsKeyPressed(KEY_BACKSPACE)) {
            if (!txt.empty()) {
                txt.pop_back();
            }
        }
        if (IsKeyPressed(KEY_ENTER)) {
            output = txt;
            txt.clear();
            to_return = true;
        }
    }
    Color text_color = feature_color(color);
    Color bg_color = background_color(color);
    Color bounds_color = border_color(color);
    draw_rectangle(x, y, width, height, bg_color);
    draw_rectangle_lines(x, y, width, height, bounds_color);
    if (t_width > width - 10) {
        x_offset -= (t_width - width) + 10;
    }
    begin_scissor(x, y, width, height);
    draw_text(txt, x + x_offset, y + y_offset, text_height, text_color);
    uint32_t fc = frame_counter % 30;
    if (selected && fc > 15) {
        draw_rectangle(x + t_width + x_offset, y + y_offset, 2, text_height, text_color);
    }
    end_scissor();
    m_current_panel.offset += height + 2;
    return to_return;
}

void GUI::text_scroll_box(arr<string_view> text, int32_t x, int32_t y, int32_t width, int32_t height, int32_t text_height, Color color, const string& id) {
    if (!m_scroll_box_scroll_amounts.contains(id)) {
        m_scroll_box_scroll_amounts[id] = 0;
    }
    int32_t& scroll_amount = m_scroll_box_scroll_amounts[id];
    x += m_current_panel.x;
    y += m_current_panel.y;
    bool hovered = CheckCollisionPointRec(GetMousePosition(), Rectangle{ float(x), float(y), float(width), float(height) });
    int32_t total_height = 0;
    vector<vector<string>> list;
    for (string_view& i : text) {
        vector<string> tmp = split_sv_lines(i, text_height, width - 5);
        total_height += (tmp.size() + 4) * text_height;
        list.push_back(tmp);
    }
    if (hovered) {
        scroll_amount += GetMouseWheelMoveV().y;
    }
    if (scroll_amount < 0) {
        scroll_amount = 0;
    }
    else if (scroll_amount > total_height) {
        scroll_amount = total_height;
    }
    draw_rectangle(x, y, width, height, background_color(color));
    draw_rectangle_lines(x, y, width, height, border_color(color));
    begin_scissor(x, y, width, height);
    Rectangle base;
    base.x = x;
    base.y = y;
    base.width = width;
    base.height = height;
    size_t i = 0;
    for (const vector<string>& j : list) {
        for (const string& k : j) {
            Rectangle rt;
            rt.x = x + 5;
            rt.y = y + i * (text_height + 2) - scroll_amount;
            rt.height = text_height;
            rt.width = (width - 5);
            if (CheckCollisionRecs(base, rt)) {
                draw_text(k, x + 5, y + i * (text_height + 2) - scroll_amount, text_height, color);
            }
            i += 1;
        }
    }
    end_scissor();
    m_current_panel.offset += height + 2;
}

int32_t GUI::text_button_scroll_box(arr<string_view> text, int32_t x, int32_t y, int32_t width, int32_t height, int32_t text_height, Color color, const string& id) {
    if (!m_scroll_box_scroll_amounts.contains(id)) {
        m_scroll_box_scroll_amounts[id] = 0;
    }
    int32_t& scroll_amount = m_scroll_box_scroll_amounts[id];
    x += m_current_panel.x;
    y += m_current_panel.y;
    bool hovered = CheckCollisionPointRec(GetMousePosition(), Rectangle{ float(x), float(y), float(width), float(height) });
    int32_t total_height = (int32_t)text.size() * (text_height + 4);
    if (hovered) {
        scroll_amount += GetMouseWheelMoveV().y;
    }
    if (scroll_amount < 0) {
        scroll_amount = 0;
    }
    else if (scroll_amount > total_height) {
        scroll_amount = total_height;
    }
    draw_rectangle(x, y, width, height, background_color(color));
    draw_rectangle_lines(x, y, width, height, border_color(color));
    begin_scissor(x, y, width, height);
    Rectangle base;
    base.x = x;
    base.y = y;
    base.width = width;
    base.height = height;
    int32_t out = -1;
    for (size_t i = 0;i < text.size(); i++) {
        Rectangle rt;
        rt.x = x + 20;
        rt.y = y + i * (text_height + 2) - scroll_amount;
        rt.height = text_height;
        rt.width = (width - 20);
        if (CheckCollisionRecs(base, rt)) {
            bool hovered = CheckCollisionPointRec(GetMousePosition(), rt);
            Color dc = color;
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && hovered) {
                dc = selected_color(color);
            }
            else if (hovered) {
                dc = hovered_color(color);
            }
            else {
                dc = color;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && hovered) {
                out = i;
            }
            draw_rectangle(x, y + i * (text_height + 2) - scroll_amount, width, text_height + 4, background_color(dc));
            draw_rectangle_lines(x, y + i * (text_height + 2) - scroll_amount, width, text_height + 4, border_color(dc));
            draw_text(text[i], x + 5, y + i * (text_height + 2) + 2 - scroll_amount, text_height, feature_color(dc));
        }
    }
    end_scissor();
    m_current_panel.offset += height + 2;
    return out;
}

void GUI::image(string_view image, int32_t x, int32_t y, int32_t width, int32_t height, Color color) {
    string nm = string(image);
    if (!m_textures.contains(nm)) {
        Texture2D texture = LoadTexture(nm.c_str());
        m_textures[nm] = texture;
    }
    m_draw_calls.push_back(DrawCall{ .kind = DRAW_CALL_DRAW_IMAGE,.text = nm,.x = x, .y = y, .width = width, .height = height,.color = color, .text_height = 0 });
    m_current_panel.offset += height + 2;
}

void GUI::reset() {
    todo("make working reset function");
}

void GUI::begin_frame() {
    m_draw_calls.clear();
    m_panel_stack.clear();
    m_current_panel = { .x = 0, .y = 0, .width = GetScreenWidth(), .height = GetScreenHeight() };
}

void GUI::end_frame() {
    frame_counter += 1;
    m_panel_stack.clear();
    m_current_panel = { .x = 0, .y = 0, .width = GetScreenWidth(), .height = GetScreenHeight() };
}

void GUI::render() {
    for (const DrawCall& i : m_draw_calls) {
        switch (i.kind) {
        case DRAW_CALL_DRAW_TEXT:
            DrawText(i.text.c_str(), i.x, i.y, i.text_height, i.color);
            break;
        case DRAW_CALL_DRAW_RECTANGLE:
            DrawRectangle(i.x, i.y, i.width, i.height, i.color);
            break;
        case DRAW_CALL_DRAW_RECTANGLE_LINES:
            DrawRectangleLines(i.x, i.y, i.width, i.height, i.color);
            break;
        case DRAW_CALL_DRAW_CIRCLE:
            DrawCircle(i.x, i.y, (i.width + i.height) / 2, i.color);
            break;
        case DRAW_CALL_DRAW_IMAGE:
        {
            Texture2D text = m_textures[i.text];
            float wr = float(text.width) / float(i.width);
            float hr = float(text.height) / float(i.height);
            float r = 1. / wr;
            if (hr < wr) {
                r = 1. / hr;
            }
            DrawTexturePro(text, { 0., 0., (float)text.width, (float)text.height }, { float(i.x), float(i.y), float(i.width) * r, float(i.height) * r }, Vector2{ 0,0 }, 0., i.color);
        }
        break;
        case DRAW_CALL_DRAW_TEXTURE: {
            float wr = float(i.texture.width) / float(i.width);
            float hr = float(i.texture.height) / float(i.height);
            float r = 1. / wr;
            if (hr < wr) {
                r = 1. / hr;
            }
            DrawTexturePro(i.texture, { 0., 0., (float)i.texture.width, (float)i.texture.height }, { float(i.x), float(i.y), float(i.width) * r, float(i.height) * r }, Vector2{ 0,0 }, 0., i.color);
        }
                                   break;
        case DRAW_CALL_BEGIN_SCISSOR:
            BeginScissorMode(i.x, i.y, i.width, i.height);
            break;
        case DRAW_CALL_END_SCISSOR:
            EndScissorMode();
            break;
        default:
            todo();
        }
    }
}

void GUI::begin_panel(int32_t x, int32_t y, int32_t width, int32_t height) {
    m_panel_stack.push_back(m_current_panel);
    m_current_panel = { x,y,width, height };
}

void GUI::end_panel() {
    assert(m_panel_stack.size() > 0);
    m_current_panel = m_panel_stack[m_panel_stack.size() - 1];
}

Color feature_color(Color base) {
    return base;
}
Color background_color(Color base) {
    Vector3 hsv = ColorToHSV(base);
    hsv.z *= 0.8;
    hsv.z = 1. - hsv.z;
    return ColorFromHSV(hsv.x, hsv.y, hsv.z);
}
Color border_color(Color base) {
    Vector3 hsv = ColorToHSV(base);
    hsv.z = 1. - hsv.z;
    return ColorFromHSV(hsv.x, hsv.y, hsv.z);
}
Color selected_color(Color base) {
    Vector3 hsv = ColorToHSV(base);
    hsv.z *= 0.95;
    return ColorFromHSV(hsv.x, hsv.y, hsv.z);
}
Color hovered_color(Color base) {
    Vector3 hsv = ColorToHSV(base);
    hsv.z *= 0.9;
    return ColorFromHSV(hsv.x, hsv.y, hsv.z);
}

DrawCall DrawCall::draw_text(string_view str, int32_t x, int32_t y, int32_t text_height, Color color) {
    DrawCall out;
    out.kind = DRAW_CALL_DRAW_TEXT;
    out.x = x;
    out.y = y;
    out.text_height = text_height;
    out.text = string(str);
    out.color = color;
    return out;
}

DrawCall DrawCall::draw_rectangle(int32_t x, int32_t y, int32_t width, int32_t height, Color color) {
    DrawCall out;
    out.kind = DRAW_CALL_DRAW_RECTANGLE;
    out.x = x;
    out.y = y;
    out.width = width;
    out.height = height;
    out.color = color;
    return out;
}

DrawCall DrawCall::draw_rectangle_lines(int32_t x, int32_t y, int32_t width, int32_t height, Color color) {
    DrawCall out;
    out.kind = DRAW_CALL_DRAW_RECTANGLE_LINES;
    out.x = x;
    out.y = y;
    out.width = width;
    out.height = height;
    out.color = color;
    return out;

}
DrawCall DrawCall::draw_circle(int32_t x, int32_t y, int32_t radius, Color color) {
    DrawCall out;
    out.kind = DRAW_CALL_DRAW_CIRCLE;
    out.x = x;
    out.y = y;
    out.width = radius;
    out.height = radius;
    out.color = color;
    return out;
}

DrawCall DrawCall::draw_image(string_view img, int32_t x, int32_t y, int32_t width, int32_t height, Color color) {
    DrawCall out;
    out.kind = DRAW_CALL_DRAW_IMAGE;
    out.text = string(img);
    out.x = x;
    out.y = y;
    out.width = width;
    out.height = height;
    out.color = color;
    return out;
}

DrawCall DrawCall::draw_texture(Texture2D texture, int32_t x, int32_t y, int32_t width, int32_t height, Color color) {
    DrawCall out;
    out.kind = DRAW_CALL_DRAW_TEXTURE;
    out.x = x;
    out.y = y;
    out.width = width;
    out.height = height;
    out.color = color;
    out.texture = texture;
    return out;
}

DrawCall DrawCall::begin_scissor(int32_t x, int32_t y, int32_t width, int32_t height) {
    DrawCall out;
    out.kind = DRAW_CALL_BEGIN_SCISSOR;
    out.x = x;
    out.y = y;
    out.width = width;
    out.height = height;
    return out;
}

DrawCall DrawCall::end_scissor() {
    DrawCall out;
    out.kind = DRAW_CALL_END_SCISSOR;
    return out;
}

void GUI::draw_text(string_view str, int32_t x, int32_t y, int32_t text_height, Color color) {
    DrawCall dc = DrawCall::draw_text(str, x, y, text_height, color);
    m_draw_calls.push_back(dc);
}
void GUI::draw_rectangle(int32_t x, int32_t y, int32_t width, int32_t height, Color color) {
    DrawCall dc = DrawCall::draw_rectangle(x, y, width, height, color);
    m_draw_calls.push_back(dc);
}
void GUI::draw_rectangle_lines(int32_t x, int32_t y, int32_t width, int32_t height, Color color) {
    DrawCall dc = DrawCall::draw_rectangle_lines(x, y, width, height, color);
    m_draw_calls.push_back(dc);
}
void GUI::draw_circle(int32_t x, int32_t y, int32_t radius, Color color) {
    DrawCall dc = DrawCall::draw_circle(x, y, radius, color);
    m_draw_calls.push_back(dc);
}
void GUI::draw_image(string_view img, int32_t x, int32_t y, int32_t width, int32_t height, Color color) {
    DrawCall dc = DrawCall::draw_image(img, x, y, width, height, color);
    m_draw_calls.push_back(dc);
}
void GUI::draw_texture(Texture2D texture, int32_t x, int32_t y, int32_t width, int32_t height, Color color) {
    DrawCall dc = DrawCall::draw_texture(texture, x, y, width, height, color);
    m_draw_calls.push_back(dc);
}
void GUI::begin_scissor(int32_t x, int32_t y, int32_t width, int32_t height) {
    DrawCall dc = DrawCall::begin_scissor(x, y, width, height);
    m_draw_calls.push_back(dc);
}
void GUI::end_scissor() {
    DrawCall dc = DrawCall::end_scissor();
    m_draw_calls.push_back(dc);
}
bool GUI::text_button(string_view text, int32_t text_height, Color color) {
    return text_button(text, m_current_panel.x + 10, m_current_panel.offset + m_current_panel.y, m_current_panel.width - 20, (text_height * 5) / 4, color);
}
void GUI::text(string_view text, int32_t text_height, Color color) {
    this->text(text, m_current_panel.x + 10, m_current_panel.offset + m_current_panel.y, m_current_panel.width - 20, (text_height * 5) / 4, color);
}
bool GUI::text_input(string& output, int32_t text_height, Color color, const string& id) {
    return text_input(output, m_current_panel.x + 10, m_current_panel.y + m_current_panel.offset, m_current_panel.width - 20, (text_height * 5) / 4, color, id);
}
void GUI::text_scroll_box(arr<string_view> text, int32_t height, int32_t text_height, Color color, const string& id) {
    text_scroll_box(text, m_current_panel.x + 10, m_current_panel.y + m_current_panel.offset, m_current_panel.width - 20, height, text_height, color, id);
}

int32_t GUI::text_button_scroll_box(arr<string_view> text, int32_t height, int32_t text_height, Color color, const string& id) {
    return text_button_scroll_box(text, m_current_panel.x + 10, m_current_panel.y + m_current_panel.offset, m_current_panel.width - 20, height, text_height, color, id);
}
void GUI::image(string_view image, int32_t height, Color color) {
    return this->image(image, m_current_panel.x + 10, m_current_panel.y + m_current_panel.offset, m_current_panel.width - 20, height, color);
}

int32_t GUI::multiline_text(string_view text, int32_t text_height, Color color) {
    vector<string> list = split_sv_lines(text, text_height, m_current_panel.width - 10);
    for (const string& i : list) {
        this->text(i, text_height, color);
    }
    return text_height * list.size();
}
vector<string> split_sv_lines(string_view text, int32_t text_height, int32_t width) {
    vector<string> out;
    string current = "";
    for (const char i : text) {
        if (i == '\n') {
            out.push_back(current);
            current.clear();
        }
        else {
            current.push_back(i);
            int32_t w = MeasureText(current.c_str(), text_height);
            if (w > width) {
                current.pop_back();
                if (!current.empty()) {
                    out.push_back(current);
                }
                current.clear();
                current.push_back(i);
            }
        }
    }
    if (!current.empty()) {
        out.push_back(current);
    }
    return out;
}

const string LOREM_IPSUM = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus lacinia efficitur lorem, a convallis tellus luctus sit amet. Phasellus nec luctus ipsum. Donec pulvinar quis nisi ut luctus. Nunc accumsan pharetra tristique. Donec quis odio augue. Suspendisse ac nisi sit amet nulla consequat placerat. Sed tincidunt lacinia nibh eu eleifend. Cras sit amet blandit nisi. Vivamus ullamcorper consectetur risus eu consectetur. Ut ut justo leo. Nulla mollis leo eu sem fermentum, non vestibulum urna congue.\n\nFusce mattis neque in arcu elementum, sit amet volutpat lectus tincidunt.Morbi non lacinia sem.In id massa lectus.Curabitur gravida urna non egestas lobortis.Quisque suscipit, mauris ut venenatis porttitor, diam dolor ornare augue, ac scelerisque nisi orci sed sem.Aenean egestas venenatis finibus.Cras non efficitur sapien.Mauris sagittis justo ante, eget sodales tortor molestie sed.Nulla eros dolor, ullamcorper nec consectetur non, bibendum vel lorem.Aenean luctus massa dui, vel semper odio tempus ut.\n\nPhasellus sit amet leo a nunc consectetur egestas.Morbi tellus purus, placerat vitae facilisis id, tincidunt ut ex.Integer sit amet imperdiet mi.Duis et egestas mi.Donec non elit vel augue finibus pharetra.Mauris et eros pulvinar, aliquam dolor eget, tristique mi.Maecenas vel diam in odio pulvinar fermentum.\n\nDonec massa mi, consectetur in dui ut, iaculis commodo elit.Duis varius rutrum odio eget dignissim.Maecenas ligula sapien, accumsan id fringilla sed, volutpat nec lacus.Morbi porttitor tellus faucibus rhoncus tincidunt.Vestibulum et tempus eros.Proin rutrum elit neque, vitae ullamcorper nunc dignissim eget.Proin vulputate mattis erat et dapibus.\n\nDonec dictum nisl ut convallis condimentum.Fusce sagittis quam feugiat dolor feugiat feugiat.Aenean vehicula commodo sem, id congue sapien.Nulla sed tellus fringilla, elementum erat at, posuere mi.Integer ut elit lectus.Nulla tincidunt accumsan scelerisque.Maecenas semper pellentesque nisi, a eleifend dui gravida quis.Ut hendrerit malesuada diam, non feugiat lectus placerat sed.Nam ac euismod nisi.Sed scelerisque ipsum sit amet ligula rhoncus, vel pharetra dolor malesuada.Nulla eleifend risus non velit egestas, ac mollis dui rutrum.";