package main
import "core:fmt"
import "gui"
import "core:strings"
_ :: strings
_ :: fmt
_ ::gui
_ :: 0
main :: proc() {
	cmds := gui.new_command_buffer();
	defer gui.destroy_command_buffer(&cmds);
	for i in 0..<10{
		gui.cmd_draw_text(&cmds, fmt.tprintf("hello:%d", i), 100, cast(i32)(i*30+30), 24,rl.WHITE);
	}
	fmt.printf("hello world\n")
	rl.InitWindow(1000, 1000, "hello window")
	rl.SetTargetFPS(61)
	for !rl.WindowShouldClose() {
		rl.BeginDrawing()
		rl.ClearBackground(rl.BLACK)
		gui.exec_command_buffer(&cmds)
		rl.EndDrawing()
	}
}
