package gui

import "base:runtime"
import "base:sanitizer"
import "core:fmt"
import "core:mem"
import "core:strings"
import "vendor:raylib"
rl :: raylib

PixelCmd :: struct {
	x, y:  i32,
	color: rl.Color,
}

@(private)
CmdDrawRectangle :: struct {
	x, y, width, height: i32,
	color:               rl.Color,
}

@(private)
CmdDrawCircle :: struct {
	x, y, radius: i32,
	color:        rl.Color,
}

@(private)
CmdDrawLine :: struct {
	x1, y1, x2, y2: i32,
	color:          rl.Color,
}

@(private)
CmdDrawText :: struct {
	text:         cstring,
	x, y, height: i32,
	color:        rl.Color,
}

@(private)
CmdDrawTriangle :: struct {
	x1, y1, x2, y2, x3, y3: i32,
	color:                  rl.Color,
}

@(private)
CmdDrawTriangleV :: struct {
	v1, v2, v3: rl.Vector2,
	color:      rl.Color,
}

@(private)
CmdBeginScissor :: struct {
	x, y, width, height: i32,
}

@(private)
CmdDrawPixels :: struct {
	pixels: []PixelCmd,
}

@(private)
CmdDrawPixel :: struct {
	x, y:  i32,
	color: rl.Color,
}

@(private)
CmdEndScissor :: struct {}

@(private)
CmdDrawTexture :: struct {
	x, y, width, height: i32,
	tint:                rl.Color,
	texture:             ^rl.Texture2D,
}

@(private)
GraphicsCommand :: union {
	CmdDrawRectangle,
	CmdDrawCircle,
	CmdDrawLine,
	CmdDrawText,
	CmdDrawTriangle,
	CmdDrawTriangleV,
	CmdDrawTexture,
	CmdBeginScissor,
	CmdEndScissor,
	CmdDrawPixels,
	CmdDrawPixel,
}

GraphicsCommandBuffer :: struct {
	commands:  [dynamic]GraphicsCommand,
	allocator: mem.Dynamic_Arena,
}

cmd_draw_rectangle :: proc(
	buffer: ^GraphicsCommandBuffer,
	x, y, width, height: i32,
	color: rl.Color,
) {
	append_elem(&buffer.commands, CmdDrawRectangle{x, y, width, height, color})
}

cmd_draw_circle :: proc(buffer: ^GraphicsCommandBuffer, x, y, radius: i32, color: rl.Color) {
	cmd := CmdDrawCircle{x, y, radius, color}
	append_elem(&buffer.commands, cmd)
}

cmd_draw_line :: proc(buffer: ^GraphicsCommandBuffer, x1, y1, x2, y2: i32, color: rl.Color) {
	cmd := CmdDrawLine{x1, y1, x2, y2, color}
	append_elem(&buffer.commands, cmd)
}

cmd_draw_triangle :: proc(
	buffer: ^GraphicsCommandBuffer,
	x1, y1, x2, y2, x3, y3: i32,
	color: rl.Color,
) {
	cmd := CmdDrawTriangle{x1, y1, x2, y2, x3, y3, color}
	append_elem(&buffer.commands, cmd)
}

cmd_draw_triangle_v :: proc(
	buffer: ^GraphicsCommandBuffer,
	v1, v2, v3: rl.Vector2,
	color: rl.Color,
) {
	cmd := CmdDrawTriangleV{v1, v2, v3, color}
	append_elem(&buffer.commands, cmd)
}


cmd_draw_text :: proc(
	buffer: ^GraphicsCommandBuffer,
	text: string,
	x, y, height: i32,
	color: rl.Color,
) {
	t2 := strings.clone_to_cstring(text, mem.dynamic_arena_allocator(&buffer.allocator))
	cmd := CmdDrawText{t2, x, y, height, color}
	append_elem(&buffer.commands, cmd)
}

cmd_begin_scissor :: proc(buffer: ^GraphicsCommandBuffer, x, y, width, height: i32) {
	cmd := CmdBeginScissor{x, y, width, height}
	append_elem(&buffer.commands, cmd)
}

cmd_end_scissor :: proc(buffer: ^GraphicsCommandBuffer) {
	cmd := CmdEndScissor{}
	append_elem(&buffer.commands, cmd)
}

cmd_draw_pixel :: proc(buffer: ^GraphicsCommandBuffer, x, y: i32, color: rl.Color) {
	cmd := CmdDrawPixel{x, y, color}
	append_elem(&buffer.commands, cmd)
}

cmd_draw_pixels :: proc(buffer: ^GraphicsCommandBuffer, pixels: []PixelCmd) {
	cmds := make_slice([]PixelCmd, len(pixels), mem.dynamic_arena_allocator(&buffer.allocator))
	cmd := CmdDrawPixels{cmds}
	append_elem(&buffer.commands, cmd)
}

cmd_draw_texture :: proc(
	buffer: ^GraphicsCommandBuffer,
	texture: ^rl.Texture2D,
	x: i32,
	y: i32,
	width: i32,
	height: i32,
	color: rl.Color,
) {
	cmd := CmdDrawTexture{x, y, width, height, color, texture}
	append_elem(&buffer.commands, cmd)
}

exec_command_buffer :: proc(buffer: ^GraphicsCommandBuffer) {
	for i in buffer.commands {
		switch v in i {
		case nil:
			break
		case CmdDrawRectangle:
			rl.DrawRectangle(v.x, v.y, v.width, v.height, v.color)
			break
		case CmdDrawCircle:
			rl.DrawCircle(v.x, v.y, cast(f32)(v.radius), v.color)
			break
		case CmdDrawLine:
			rl.DrawLine(v.x1, v.y1, v.x2, v.y2, v.color)
			break
		case CmdDrawText:
			rl.DrawText(v.text, v.x, v.y, v.height, v.color)
			break
		case CmdDrawTriangle:
			v1 := rl.Vector2{cast(f32)v.x1, cast(f32)v.y1}
			v2 := rl.Vector2{cast(f32)v.x2, cast(f32)v.y2}
			v3 := rl.Vector2{cast(f32)v.x3, cast(f32)v.y3}
			rl.DrawTriangle(v1, v2, v3, v.color)
			break
		case CmdDrawTriangleV:
			rl.DrawTriangle(v.v1, v.v2, v.v3, v.color)
			break
		case CmdBeginScissor:
			rl.BeginScissorMode(v.x, v.y, v.width, v.height)
			break
		case CmdEndScissor:
			rl.EndScissorMode()
			break
		case CmdDrawPixels:
			for i in v.pixels {
				rl.DrawPixel(i.x, i.y, i.color)
			}
			break
		case CmdDrawPixel:
			rl.DrawPixel(v.x, v.y, v.color)
			break
		case CmdDrawTexture:
			rl.DrawTexturePro(
				v.texture^,
				rl.Rectangle{0., 0., cast(f32)v.texture.width, cast(f32)v.texture.height},
				rl.Rectangle{cast(f32)v.x, cast(f32)v.y, cast(f32)v.width, cast(f32)v.height},
				{0.0, 0.0},
				0.0,
				v.tint,
			)
			break
		}

	}
}

clear_command_buffer :: proc(buffer: ^GraphicsCommandBuffer) {
	mem.dynamic_arena_free_all(&buffer.allocator)
	clear_dynamic_array(&buffer.commands)
}

destroy_command_buffer :: proc(buffer: ^GraphicsCommandBuffer) {
	mem.dynamic_arena_destroy(&buffer.allocator)
	delete_dynamic_array(buffer.commands)
}

new_command_buffer :: proc() -> GraphicsCommandBuffer {
	out: GraphicsCommandBuffer
	mem.dynamic_arena_init(&out.allocator)
	return out
}

GuiObjectState :: enum {
	NotSelected,
	Hovered,
	Selected,
}

GuiObject :: struct {
	type:                    GuiObjectType,
	id:                      string,
	text:                    []string,
	x, y, width, height:     i32,
	text_height:             i32,
	chidren:                 [dynamic]^GuiObject,
	parent:                  ^GuiObject,
	bmp_offset:              i32,
	padding:                 i32,
	position_set_explicitly: bool,
	text_color:              rl.Color,
	outline_color:           rl.Color,
	bg_color:                rl.Color,
	state:                   GuiObjectState,
	texture:                 ^rl.Texture2D,
}


GuiObjectType :: enum {
	Button,
	Div,
	ScrollBox,
	Text,
	Image,
}

GuiScrollBoxData :: struct {
	scroll_offset_px: i32,
	height_px:        i32,
}

GuiContext :: struct {
	scroll_box_data:     map[string]GuiScrollBoxData,
	arena:               ^mem.Dynamic_Arena,
	roots:               [dynamic]^GuiObject,
	x, y, width, height: i32,
	current_object:      ^GuiObject,
	object_stack:        [dynamic]^GuiObject,
	images:              map[string]rl.Texture2D,
}

gui_allocator :: proc(ctx: ^GuiContext) -> runtime.Allocator {
	return mem.dynamic_arena_allocator(ctx.arena)
}

gui_object_id :: proc(ctx: ^GuiContext, idx: i32, loc: runtime.Source_Code_Location) -> string {
	return fmt.aprintf(
		"%d-%s-%s-%d",
		idx,
		loc.file_path,
		loc.procedure,
		loc.line,
		allocator = gui_allocator(ctx),
	)
}


gui_new_object :: proc(ctx: ^GuiContext, type: GuiObjectType, name: string) -> ^GuiObject {
	obj := new(GuiObject, gui_allocator(ctx))
	runtime.memset(obj, 0, size_of(obj^))
	obj.id = name
	obj.parent = ctx.current_object
	obj.type = type
	obj.chidren = make([dynamic]^GuiObject, gui_allocator((ctx)))
	obj.text_color = rl.WHITE
	obj.bg_color = rl.GRAY
	obj.outline_color = rl.BLACK
	obj.padding = 5
	if (ctx.current_object != nil) {
		append(&ctx.current_object.chidren, obj)
	}
	return obj
}

gui_begin_root :: proc(
	ctx: ^GuiContext,
	x, y, width, height: i32,
	idx := 0,
	loc := #caller_location,
) {
	assert(
		ctx.current_object == nil,
		"cannot create a root with an object already established as a root",
	)
	id := gui_object_id(ctx, cast(i32)idx, loc)
	obj := gui_new_object(ctx, GuiObjectType.Div, id)
	obj.x = x
	obj.y = y
	obj.width = width
	obj.height = height
	append(&ctx.object_stack, ctx.current_object)
	append(&ctx.roots, obj)
	ctx.current_object = obj
}


gui_end_root :: proc(ctx: ^GuiContext) {
	ctx.current_object = pop(&ctx.object_stack)
	assert(ctx.current_object == nil, "root must be the final object in existence")
}

gui_begin_div :: proc(ctx: ^GuiContext, idx := 0, loc := #caller_location) {
	id := gui_object_id(ctx, cast(i32)idx, loc)
	obj := gui_new_object(ctx, GuiObjectType.Div, id)
	obj.x = ctx.current_object.x + ctx.current_object.padding
	obj.y = ctx.current_object.y + ctx.current_object.bmp_offset + ctx.current_object.padding
	obj.width = ctx.current_object.width - ctx.current_object.padding * 2
	append(&ctx.object_stack, ctx.current_object)
	ctx.current_object = obj
}

gui_begin_div_exp :: proc(
	ctx: ^GuiContext,
	dx: i32,
	dy: i32,
	width: i32,
	idx := 0,
	loc := #caller_location,
) {
	id := gui_object_id(ctx, cast(i32)idx, loc)
	obj := gui_new_object(ctx, GuiObjectType.Div, id)
	obj.x = ctx.current_object.x + ctx.current_object.padding + dx
	obj.y = ctx.current_object.y + ctx.current_object.bmp_offset + ctx.current_object.padding + dy
	obj.width = width
	obj.position_set_explicitly = true
	append(&ctx.object_stack, ctx.current_object)
	ctx.current_object = obj

}

gui_end_div :: proc(ctx: ^GuiContext) {
	prev := ctx.current_object
	old_height := prev.height
	prev.height = prev.bmp_offset
	if prev.height < old_height {
		prev.height = old_height
	}
	ctx.current_object = pop(&ctx.object_stack)
	assert(ctx.current_object != nil)
	if !prev.position_set_explicitly {
		ctx.current_object.bmp_offset += prev.height
	}
}

gui_button :: proc(
	ctx: ^GuiContext,
	text: string,
	text_height: i32,
	idx := 0,
	loc := #caller_location,
) -> bool {
	id := gui_object_id(ctx, cast(i32)idx, loc)
	obj := gui_new_object(ctx, GuiObjectType.Button, id)
	obj.x = ctx.current_object.x + ctx.current_object.padding
	obj.y = ctx.current_object.y + ctx.current_object.bmp_offset + ctx.current_object.padding
	obj.width = ctx.current_object.width - ctx.current_object.padding * 2
	obj.text = split_text_lines(text, text_height, obj.width - obj.padding * 2, gui_allocator(ctx))
	obj.height =
		text_height if len(obj.text) == 0 else cast(i32)len(obj.text) * text_height + obj.padding * 2
	ctx.current_object.bmp_offset += obj.height + obj.padding
	obj.text_height = text_height
	x := cast(i32)rl.GetMouseX()
	y := cast(i32)rl.GetMouseY()
	hit := (x >= obj.x && x < obj.x + obj.width) && (y >= obj.y && y < obj.y + obj.height)
	if !rl.CheckCollisionPointRec(rl.GetMousePosition(), gui_object_bounds(obj.parent)) &&
	   obj.parent.type == GuiObjectType.ScrollBox {
		hit = false
	}
	if hit {
		if rl.IsMouseButtonDown(rl.MouseButton.LEFT) {
			obj.state = GuiObjectState.Selected
		} else {
			obj.state = GuiObjectState.Hovered
		}
	}
	return hit && rl.IsMouseButtonReleased(rl.MouseButton.LEFT)
}

gui_text :: proc(
	ctx: ^GuiContext,
	text: string,
	text_height: i32,
	idx := 0,
	loc := #caller_location,
) {
	id := gui_object_id(ctx, cast(i32)idx, loc)
	obj := gui_new_object(ctx, GuiObjectType.Text, id)
	obj.x = ctx.current_object.x + ctx.current_object.padding
	obj.y = ctx.current_object.y + ctx.current_object.bmp_offset + ctx.current_object.padding
	obj.width = ctx.current_object.width - ctx.current_object.padding * 2
	obj.text = split_text_lines(text, text_height, obj.width - obj.padding * 2, gui_allocator(ctx))
	obj.height =
		text_height if len(obj.text) == 0 else cast(i32)len(obj.text) * text_height + obj.padding * 2
	obj.text_height = text_height
	ctx.current_object.bmp_offset += obj.height + obj.padding


}

gui_button_exp :: proc(
	ctx: ^GuiContext,
	text: string,
	text_height: i32,
	dx: i32,
	dy: i32,
	width: i32,
	idx := 0,
	loc := #caller_location,
) -> bool {
	id := gui_object_id(ctx, cast(i32)idx, loc)
	obj := gui_new_object(ctx, GuiObjectType.Button, id)
	obj.x = ctx.current_object.x + ctx.current_object.padding + dx
	obj.y = ctx.current_object.y + ctx.current_object.bmp_offset + ctx.current_object.padding + dy
	obj.width = width
	obj.text = split_text_lines(text, text_height, obj.width - obj.padding * 2, gui_allocator(ctx))
	obj.height =
		text_height if len(obj.text) == 0 else cast(i32)len(obj.text) * text_height + obj.padding * 2
	obj.text_height = text_height
	obj.position_set_explicitly = true
	x := cast(i32)rl.GetMouseX()
	y := cast(i32)rl.GetMouseY()
	hit := (x >= obj.x && x < obj.x + obj.width) && (y >= obj.y && y < obj.y + obj.height)
	if !rl.CheckCollisionPointRec(rl.GetMousePosition(), gui_object_bounds(obj.parent)) &&
	   obj.parent.type == GuiObjectType.ScrollBox {
		hit = false
	}
	if hit {
		if rl.IsMouseButtonDown(rl.MouseButton.LEFT) {
			obj.state = GuiObjectState.Selected
		} else {
			obj.state = GuiObjectState.Hovered
		}
	}
	return hit && rl.IsMouseButtonReleased(rl.MouseButton.LEFT)
}

gui_text_exp :: proc(
	ctx: ^GuiContext,
	text: string,
	text_height: i32,
	dx: i32,
	dy: i32,
	width: i32,
	idx := 0,
	loc := #caller_location,
) {
	id := gui_object_id(ctx, cast(i32)idx, loc)
	obj := gui_new_object(ctx, GuiObjectType.Text, id)
	obj.x = ctx.current_object.x + ctx.current_object.padding + dx
	obj.y = ctx.current_object.y + ctx.current_object.bmp_offset + ctx.current_object.padding + dy
	obj.width = width
	obj.text = split_text_lines(text, text_height, obj.width - obj.padding * 2, gui_allocator(ctx))
	obj.text_height = text_height
	obj.height =
		text_height if len(obj.text) == 0 else cast(i32)len(obj.text) * text_height + obj.padding * 2
	obj.position_set_explicitly = true
}

gui_create :: proc() -> ^GuiContext {
	out: ^GuiContext = new(GuiContext)
	out.arena = new(mem.Dynamic_Arena)
	mem.dynamic_arena_init(out.arena)
	out.object_stack = make([dynamic]^GuiObject)
	out.roots = make([dynamic]^GuiObject)
	out.scroll_box_data = make(map[string]GuiScrollBoxData)
	return out
}

gui_destroy :: proc(ctx: ^GuiContext) {
	mem.dynamic_arena_destroy(ctx.arena)
	free(ctx.arena)
	delete(ctx.object_stack)
	delete(ctx.roots)
	for i, j in ctx.scroll_box_data {
		delete(i)
	}
	delete(ctx.scroll_box_data)

	free(ctx)
}

gui_begin_frame :: proc(
	ctx: ^GuiContext,
	x: i32 = 0,
	y: i32 = 0,
	width: i32 = -1,
	height: i32 = -1,
) {
	width := width
	height := height
	if (width == -1) {
		width = cast(i32)rl.GetScreenWidth()
	}
	if (height == -1) {
		height = cast(i32)rl.GetScreenHeight()
	}
	ctx.x = x
	ctx.y = y
	ctx.width = width
	ctx.height = height
	clear(&ctx.object_stack)
	clear(&ctx.roots)
	mem.dynamic_arena_free_all(ctx.arena)
}

gui_end_frame :: proc(ctx: ^GuiContext) {}

gui_render :: proc(ctx: ^GuiContext) {
	buffer := gui_render_to_buffer(ctx)
	defer destroy_command_buffer(&buffer)
	exec_command_buffer(&buffer)
}

gui_render_to_buffer :: proc(ctx: ^GuiContext) -> GraphicsCommandBuffer {
	out := new_command_buffer()
	for i in ctx.roots {
		gui_draw_object(i, &out)
	}
	return out
}

measure_text_height :: proc(text: string, text_height: i32, max_width: i32) -> i32 {
	out := text_height
	alloc: runtime.Arena
	defer runtime.arena_destroy(&alloc)
	count := split_text_lines(text, text_height, max_width, runtime.arena_allocator(&alloc))
	if len(count) > 0 {
		out = cast(i32)len(count) * text_height
	}
	return out
}

split_text_lines :: proc(
	text: string,
	text_height: i32,
	max_width: i32,
	allocator: runtime.Allocator,
) -> []string {
	out := make([dynamic]string, allocator)
	current := strings.builder_make(allocator)
	width: i32 = 0
	for i in text {
		w := measure_rune_width(i, text_height)
		if i == '\n' || width + w >= max_width {
			append(&out, strings.clone(strings.to_string(current), allocator))
			width = 0
			strings.builder_reset(&current)
			if i != '\n' {
				strings.write_rune(&current, i)
			}
		} else {
			strings.write_rune(&current, i)
			width += w
		}
	}
	tmp := strings.to_string(current)
	if len(tmp) > 0 {
		t2 := strings.clone(tmp, allocator)
		append(&out, t2)
	}
	return out[:]
}

measure_rune_width :: proc(rn: rune, size: i32) -> i32 {
	if rn == '\n' {
		return 0
	} else if rn == '\t' {
		return size * 3
	} else {
		return (size * 2) / 5
	}
}

gui_draw_object :: proc(obj: ^GuiObject, cmds: ^GraphicsCommandBuffer) {
	switch obj.type {
	case .Button:
		cmd_draw_rectangle(
			cmds,
			obj.x,
			obj.y,
			obj.width,
			obj.height,
			gui_trans_color(obj.bg_color, obj.state),
		)
		dy: i32 = 0
		for i in obj.text {
			cmd_draw_text(
				cmds,
				i,
				obj.x + obj.padding,
				obj.y + dy + obj.padding,
				obj.text_height,
				gui_trans_color(obj.text_color, obj.state),
			)
			dy += obj.text_height
		}
		break
	case .Div:
		for i in obj.chidren {
			gui_draw_object(i, cmds)
		}
		break
	case .ScrollBox:
		cmd_begin_scissor(cmds, obj.x, obj.y, obj.width, obj.height)
		bds := gui_object_bounds(obj)
		for i in obj.chidren {
			tmp := gui_object_bounds(i)
			if rl.CheckCollisionRecs(bds, tmp) {
				gui_draw_object(i, cmds)
			}
		}
		cmd_end_scissor(cmds)
		break
	case .Image:
		cmd_draw_texture(cmds, obj.texture, obj.x, obj.y, obj.width, obj.height, obj.bg_color)
		break
	case .Text:
		cmd_draw_rectangle(
			cmds,
			obj.x,
			obj.y,
			obj.width,
			obj.height,
			gui_trans_color(obj.bg_color, obj.state),
		)
		dy: i32 = 0
		for i in obj.text {
			cmd_draw_text(
				cmds,
				i,
				obj.x + obj.padding,
				obj.y + dy + obj.padding,
				obj.text_height,
				gui_trans_color(obj.text_color, obj.state),
			)
			dy += obj.text_height
		}
		break
	}
}


gui_object_bounds :: proc(obj: ^GuiObject) -> rl.Rectangle {
	return rl.Rectangle{cast(f32)obj.x, cast(f32)obj.y, cast(f32)obj.width, cast(f32)obj.height}
}

gui_trans_color :: proc(color: rl.Color, state: GuiObjectState) -> rl.Color {
	t_out := cast([4]f32)color
	base_a := t_out.a
	switch state {
	case .NotSelected:
		t_out = t_out * 1.
		break
	case .Hovered:
		t_out = t_out * 0.9
		break
	case .Selected:
		t_out = t_out * 0.8
		break
	}
	t_out.a = base_a
	return cast(rl.Color)cast([4]u8)t_out
}

gui_centered :: proc(object_dim: i32, within_dim: i32 = -1) -> i32 {
	within_dim := within_dim
	if (within_dim == -1) {
		within_dim = rl.GetScreenWidth()
	}
	return (within_dim - object_dim) / 2
}

gui_centered_v :: proc(object_dim: i32, within_dim: i32 = -1) -> i32 {
	within_dim := within_dim
	if (within_dim == -1) {
		within_dim = rl.GetScreenHeight()
	}
	return (within_dim - object_dim) / 2
}

trans :: proc(v: i32) -> i32 {
	rat := (cast(f32)rl.GetScreenWidth() / 1920)
	out := cast(f32)v * rat
	return cast(i32)out
}

trans_v :: proc(v: i32) -> i32 {
	rat := (cast(f32)rl.GetScreenHeight() / 1080)
	out := cast(f32)v * rat
	return cast(i32)out
}

gui_centered_trans :: proc(object_dim: i32, within_dim: i32 = -1) -> i32 {
	within_dim := within_dim
	if (within_dim == -1) {
		within_dim = rl.GetScreenWidth()
	} else {
		within_dim = trans(within_dim)
	}
	return (within_dim - trans(object_dim)) / 2
}

gui_centered_v_trans :: proc(object_dim: i32, within_dim: i32 = -1) -> i32 {
	within_dim := within_dim
	if (within_dim == -1) {
		within_dim = rl.GetScreenHeight()
	} else {
		within_dim = trans(within_dim)
	}
	return (within_dim - trans(object_dim)) / 2
}

gui_begin_root_centered :: proc(
	ctx: ^GuiContext,
	width: i32,
	height: i32,
	idx := 0,
	loc := #caller_location,
) {
	gui_begin_root(ctx, gui_centered(width), gui_centered_v(height), width, height, idx, loc)
}

gui_end_root_centered :: proc(ctx: ^GuiContext) {
	gui_end_root(ctx)
}

gui_shift_for_exp :: proc(ctx: ^GuiContext) {
	cur := ctx.current_object
	assert(cur != nil)
	assert(len(cur.chidren) > 0)
	last := cur.chidren[len(cur.chidren) - 1]
	dy := last.y - cur.y + last.height
	if dy >= cur.bmp_offset {
		cur.bmp_offset = dy
	}
}

gui_next_pos_for_exp :: proc(ctx: ^GuiContext) -> i32 {
	cur := ctx.current_object
	assert(cur != nil)
	assert(len(cur.chidren) > 0)
	last := cur.chidren[len(cur.chidren) - 1]
	dx := last.x - cur.x + last.width
	return dx
}


gui_begin_scrollbox :: proc(ctx: ^GuiContext, height: i32, idx := 0, loc := #caller_location) {
	id := gui_object_id(ctx, cast(i32)idx, loc)
	obj := gui_new_object(ctx, GuiObjectType.ScrollBox, id)
	obj.x = ctx.current_object.x + ctx.current_object.padding
	obj.y = ctx.current_object.y + ctx.current_object.bmp_offset + ctx.current_object.padding
	obj.width = ctx.current_object.width - ctx.current_object.padding * 2
	obj.height = height
	if id in ctx.scroll_box_data {

		if rl.CheckCollisionPointRec(rl.GetMousePosition(), gui_object_bounds(obj)) {
			tmp := ctx.scroll_box_data[id]
			tmp.scroll_offset_px -= cast(i32)rl.GetMouseWheelMoveV().y
			if tmp.scroll_offset_px > 0 {
				tmp.scroll_offset_px = 0
			}
			if tmp.scroll_offset_px < -tmp.height_px {
				tmp.scroll_offset_px = -tmp.height_px
			}
			ctx.scroll_box_data[id] = tmp
		}
		obj.bmp_offset = ctx.scroll_box_data[id].scroll_offset_px
	} else {
		map_insert(&ctx.scroll_box_data, strings.clone(id), GuiScrollBoxData{})
	}
	append(&ctx.object_stack, ctx.current_object)
	ctx.current_object = obj
}

gui_begin_scrollbox_exp :: proc(
	ctx: ^GuiContext,
	dx: i32,
	dy: i32,
	width: i32,
	height: i32,
	idx := 0,
	loc := #caller_location,
) {
	id := gui_object_id(ctx, cast(i32)idx, loc)
	obj := gui_new_object(ctx, GuiObjectType.ScrollBox, id)
	obj.x = ctx.current_object.x + ctx.current_object.padding + dx
	obj.y = ctx.current_object.y + ctx.current_object.bmp_offset + ctx.current_object.padding + dy
	obj.width = width
	obj.height = height
	if id in ctx.scroll_box_data {
		if rl.CheckCollisionPointRec(rl.GetMousePosition(), gui_object_bounds(obj)) {
			tmp := ctx.scroll_box_data[id]
			tmp.scroll_offset_px -= cast(i32)rl.GetMouseWheelMoveV().y
			if tmp.scroll_offset_px > 0 {
				tmp.scroll_offset_px = 0
			}
			if tmp.scroll_offset_px < -tmp.height_px {
				tmp.scroll_offset_px = -tmp.height_px
			}
			ctx.scroll_box_data[id] = tmp
		}
		obj.bmp_offset = ctx.scroll_box_data[id].scroll_offset_px
	} else {
		map_insert(&ctx.scroll_box_data, strings.clone(id), GuiScrollBoxData{})
	}
	obj.position_set_explicitly = true
	append(&ctx.object_stack, ctx.current_object)
	ctx.current_object = obj

}

gui_end_scrollbox :: proc(ctx: ^GuiContext) {
	prev := ctx.current_object
	base := ctx.scroll_box_data[prev.id]
	base.height_px = prev.bmp_offset * 2 - prev.height + prev.padding * 2 - base.scroll_offset_px
	if len(prev.chidren) > 0 {
		base.height_px -= prev.chidren[len(prev.chidren) - 1].height
	}
	if base.height_px < 0 {
		base.height_px = 0
	}
	ctx.scroll_box_data[prev.id] = base
	ctx.current_object = pop(&ctx.object_stack)
	assert(ctx.current_object != nil)
	if !prev.position_set_explicitly {
		ctx.current_object.bmp_offset += prev.height + prev.padding * 2
	}
}

gui_image :: proc(ctx: ^GuiContext, name: string, idx: i32 = 0, loc := #caller_location) {
	id := gui_object_id(ctx, idx, loc)
	obj := gui_new_object(ctx, GuiObjectType.Image, id)
	if !(name in ctx.images) {
		text := rl.LoadTexture(strings.clone_to_cstring(name, gui_allocator(ctx)))
		tname := strings.clone(name)
		map_insert(&ctx.images, name, text)
	}
	texture := &ctx.images[name]
	obj.texture = texture
	rat := cast(f32)texture.height / cast(f32)texture.width
	obj.x = ctx.current_object.x + ctx.current_object.padding
	obj.y = ctx.current_object.y + ctx.current_object.bmp_offset + ctx.current_object.padding
	obj.width = ctx.current_object.width - ctx.current_object.padding * 2
	obj.height = cast(i32)(rat * cast(f32)obj.width)
	ctx.current_object.bmp_offset += obj.height + obj.padding
}
