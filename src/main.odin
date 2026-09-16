package main
import "base:runtime"
import "core:fmt"
import "core:mem"
import "core:strings"
import "gui"
import "vendor:raylib"
rl :: raylib
_ :: strings
_ :: fmt
_ :: gui
_ :: 0
main :: proc() {
	track: mem.Tracking_Allocator
	mem.tracking_allocator_init(&track, context.allocator)
	defer mem.tracking_allocator_destroy(&track)
	context.allocator = mem.tracking_allocator(&track)
	{
		rl.InitWindow(1000, 1000, "hello window!")
		defer rl.CloseWindow()
		ctx := gui.gui_create()
		defer gui.gui_destroy(ctx)
		for !rl.WindowShouldClose() {
			gui.gui_begin_frame(ctx)
			{
				gui.gui_begin_root(ctx, 100, 100, 300, 500)
				{
					gui.gui_begin_div(ctx)
					gui.gui_text(ctx, "hello world!", 20)
					if gui.gui_button(ctx, "click to exit", 40) {
						break
					}
					gui.gui_end_div(ctx)
				}
				gui.gui_end_root(ctx)
			}
			gui.gui_end_frame(ctx)
			rl.BeginDrawing()
			rl.ClearBackground(rl.BLACK)
			gui.gui_render(ctx)
			rl.EndDrawing()
		}

	}
	for _, leak in track.allocation_map {
		fmt.printf("%v leaked %m\n", leak.location, leak.size)
	}
}
