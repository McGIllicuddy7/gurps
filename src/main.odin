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
	gctx := gui.gui_create()
	defer gui.gui_destroy(gctx)
	count := 0
	for !rl.WindowShouldClose() {
		gui.gui_begin_frame(gctx)
		gui.gui_begin_root_centered(gctx, 500, 800)
		gui.gui_begin_div(gctx)
		gui.gui_text(gctx, "hello window!", 24)
		gui.gui_begin_scrollbox(gctx, 500)
		tcount := count
		for i in 0 ..< tcount {
			if gui.gui_button_thumbnail(
				gctx,
				"explosion.png",
				fmt.tprint(i, gui.LOREM_IPSUM),
				20,
			) {
				count = i
			}
		}
		gui.gui_end_scrollbox(gctx)
		if gui.gui_button_exp(gctx, "inc count", 20, 0, 2, 150) {
			count += 1
		}
		if gui.gui_button_exp(gctx, "dec count", 20, gui.gui_next_pos_for_exp(gctx), 2, 150) {
			count -= 1
			if count < 0 {
				count = 0
			}
		}
		if gui.gui_button_exp(gctx, "reset count", 20, gui.gui_next_pos_for_exp(gctx), 2, 150) {
			count = 0
		}
		gui.gui_text_exp(gctx, fmt.tprint(count), 20, gui.gui_next_pos_for_exp(gctx), 2, 50)

		gui.gui_shift_for_exp(gctx)
		gui.gui_end_div(gctx)
		gui.gui_end_root_centered(gctx)
		gui.gui_end_frame(gctx)
		rl.BeginDrawing()
		rl.ClearBackground(rl.BLACK)
		gui.gui_render(gctx)
		rl.EndDrawing()
	}
}
