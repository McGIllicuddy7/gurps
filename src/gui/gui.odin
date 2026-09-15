package gui

import "core:container/queue"
import "vendor:raylib"
rl :: raylib

DrawRectangle :: struct {
	x, y, width, height: i32,
	color:               rl.Color,
}
DrawCircle :: struct {}
DrawLine :: struct {}
DrawText :: struct {}
DrawTriangle :: struct {}
BeginScissor :: struct {}
EndScissor :: struct {}

GraphicsCommand :: union {
	DrawRectangle,
	DrawCircle,
	DrawLine,
	DrawText,
	DrawTriangle,
	BeginScissor,
	EndScissor,
}
