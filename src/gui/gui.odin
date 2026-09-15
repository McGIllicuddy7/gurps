package gui

import "base:runtime"
import "core:strings"
import "vendor:raylib"
rl :: raylib

PixelCmd::struct {
	x,y:i32,
	color:rl.Color
}
@private
CmdDrawRectangle :: struct {
	x, y, width, height: i32,
	color:               rl.Color,
}

@private
CmdDrawCircle :: struct {
	x,y, radius:i32,
	color:rl.Color
}

@private
CmdDrawLine :: struct {
	x1,y1,x2,y2:i32, 
	color:rl.Color
}

@private
CmdDrawText :: struct {
	text:cstring, 
	x,y,height:i32,
	color:rl.Color
}

@private
CmdDrawTriangle :: struct {
	x1,y1,x2,y2,x3,y3:i32,
	color:rl.Color
}

@private
CmdDrawTriangleV :: struct {
	v1,v2,v3:rl.Vector2, 
	color:rl.Color
}

@private
CmdBeginScissor :: struct {
	x,y,width,height:i32
}

@private
CmdDrawPixels ::struct {
	pixels:[]PixelCmd
}

@private
CmdDrawPixel :: struct {
	x,y:i32, color:rl.Color
}

@private
CmdEndScissor :: struct {}

@private
GraphicsCommand :: union {
	CmdDrawRectangle,
	CmdDrawCircle,
	CmdDrawLine,
	CmdDrawText,
	CmdDrawTriangle,
	CmdDrawTriangleV,
	CmdBeginScissor,
	CmdEndScissor,
	CmdDrawPixels, 
	CmdDrawPixel
}

GraphicsCommandBuffer :: struct {
	commands:[dynamic]GraphicsCommand,
	allocator:runtime.Arena
}

cmd_draw_rectangle::proc(buffer:^GraphicsCommandBuffer, x,y,width,height:i32, color:rl.Color){
	append_elem(&buffer.commands, CmdDrawRectangle{x,y,width,height,color});
}

cmd_draw_circle::proc(buffer:^GraphicsCommandBuffer, x,y,radius:i32, color:rl.Color){
	cmd := CmdDrawCircle{x,y,radius,color};
	append_elem(&buffer.commands,cmd);
}

cmd_draw_line::proc(buffer:^GraphicsCommandBuffer, x1,y1, x2,y2:i32, color:rl.Color){
	cmd := CmdDrawLine{x1,y1,x2,y2,color};
	append_elem(&buffer.commands, cmd);
}

cmd_draw_triangle::proc(buffer:^GraphicsCommandBuffer, x1,y1,x2,y2,x3,y3:i32, color:rl.Color){
	cmd := CmdDrawTriangle{x1,y1,x2,y2,x3,y3,color};
	append_elem(&buffer.commands, cmd);
}

cmd_draw_triangle_v::proc(buffer:^GraphicsCommandBuffer, v1,v2,v3:rl.Vector2, color:rl.Color){
	cmd := CmdDrawTriangleV{v1,v2,v3,color};
	append_elem(&buffer.commands, cmd);
}

cmd_draw_text::proc(buffer:^GraphicsCommandBuffer,text:string, x,y,height:i32, color:rl.Color){
	t2 := strings.clone_to_cstring(text, runtime.arena_allocator(&buffer.allocator));
	cmd := CmdDrawText{t2, x,y,height,color};
	append_elem(&buffer.commands, cmd);
}

cmd_begin_scissor::proc(buffer:^GraphicsCommandBuffer, x,y,width,height:i32){
	cmd := CmdBeginScissor{x,y,width,height}
	append_elem(&buffer.commands, cmd);
}

cmd_end_scissor::proc(buffer:^GraphicsCommandBuffer){
	cmd := CmdEndScissor{};
	append_elem(&buffer.commands,cmd);
}

cmd_draw_pixel::proc(buffer:^GraphicsCommandBuffer, x,y:i32,color:rl.Color){
	cmd := CmdDrawPixel{x,y,color};
	append_elem(&buffer.commands, cmd);
}

cmd_draw_pixels::proc(buffer:^GraphicsCommandBuffer, pixels:[]PixelCmd){
	cmds :=make_slice([]PixelCmd, len(pixels), runtime.arena_allocator(&buffer.allocator));
	cmd := CmdDrawPixels{cmds};
	append_elem(&buffer.commands, cmd);
}

exec_command_buffer::proc(buffer:^GraphicsCommandBuffer){
	for i in buffer.commands{
		switch v in i{
			case nil:	
			break;
			case CmdDrawRectangle:	
				rl.DrawRectangle(v.x, v.y, v.width, v.height, v.color);
			break;
			case CmdDrawCircle:
				rl.DrawCircle(v.x, v.y, cast(f32)(v.radius), v.color)
			break;
			case CmdDrawLine:
				rl.DrawLine(v.x1, v.y1, v.x2, v.y2, v.color);
			break;
			case CmdDrawText:
				rl.DrawText(v.text,v.x, v.y, v.height, v.color)
			break;
			case CmdDrawTriangle:
				v1 := rl.Vector2{cast(f32)v.x1, cast(f32)v.y1}
				v2 := rl.Vector2{cast(f32)v.x2, cast(f32)v.y2}
				v3 := rl.Vector2{cast(f32)v.x3, cast(f32)v.y3}
				rl.DrawTriangle(v1,v2,v3, v.color);
			break;
			case CmdDrawTriangleV:
				rl.DrawTriangle(v.v1, v.v2, v.v3, v.color);
			break
			case CmdBeginScissor:
				rl.BeginScissorMode(v.x, v.y, v.width,v.height)
			break;
			case CmdEndScissor: 
				rl.EndScissorMode()
			break;
			case CmdDrawPixels:
				for i in v.pixels{
					rl.DrawPixel(i.x, i.y, i.color)
				}
			break
			case CmdDrawPixel:
				rl.DrawPixel(v.x,v.y, v.color)
			break
		}	
	}
}

clear_command_buffer::proc(buffer:^GraphicsCommandBuffer){
	runtime.arena_free_all(&buffer.allocator)
	clear_dynamic_array(&buffer.commands)
}

destroy_command_buffer::proc(buffer:^GraphicsCommandBuffer){
	runtime.arena_destroy(&buffer.allocator)
	delete_dynamic_array(buffer.commands)
}

new_command_buffer::proc()->GraphicsCommandBuffer{
	out:GraphicsCommandBuffer
	return out
}


GUIButton ::struct {}
GUIText ::struct {}
GUIScrollBox:: struct {}
GUIObject ::union{}
GUIContext::struct {
	object_cache:map[string]^GUIObject
}
