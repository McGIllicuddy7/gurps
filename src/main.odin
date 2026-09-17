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
	debug_main(dbg_main)
}
dbg_main :: proc() {
	rl.InitWindow(1000, 1000, "hello window!")
	defer rl.CloseWindow()
	ctx := gui.gui_create()
	defer gui.gui_destroy(ctx)
	count := 0
	for !rl.WindowShouldClose() {
		gui.gui_begin_frame(ctx)
		{
			gui.gui_begin_root_centered(ctx, 500, 1000)
			{
				gui.gui_begin_div(ctx)
				gui.gui_text(ctx, "hello world!", 20)
				if gui.gui_button(ctx, "click to exit", 40) {
					break
				}
				gui.gui_begin_scrollbox(ctx, 200)
				tcount := count
				for i in 0 ..< tcount {
					if gui.gui_button(ctx, fmt.tprint(i), 20) {
						count = i
					}
				}
				gui.gui_end_scrollbox(ctx)
				gui.gui_image(ctx, "explosion.png")
				if gui.gui_button_exp(ctx, "inc count", 20, 0, 0, 140) {
					count += 1
				}
				if gui.gui_button_exp(
					ctx,
					"dec count",
					20,
					gui.gui_next_pos_for_exp(ctx),
					0,
					140,
				) {
					count -= 1
					if count < 0 {
						count = 0
					}
				}
				if gui.gui_button_exp(
					ctx,
					"count reset",
					20,
					gui.gui_next_pos_for_exp(ctx),
					0,
					160,
				) {
					count = 0
				}
				gui.gui_text_exp(ctx, fmt.tprint(count), 20, gui.gui_next_pos_for_exp(ctx), 0, 20)
				gui.gui_shift_for_exp(ctx)
				gui.gui_text(ctx, "end", 20)
				gui.gui_end_div(ctx)
			}
			gui.gui_end_root_centered(ctx)
		}
		gui.gui_end_frame(ctx)
		rl.BeginDrawing()
		rl.ClearBackground(rl.BLACK)
		gui.gui_render(ctx)
		rl.EndDrawing()
	}
}
