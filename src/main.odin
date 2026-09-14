package main
import "core:fmt"
_ :: fmt

_ :: 0
main :: proc() {
	fmt.printf("hello world\n")
	rl.InitWindow(1000, 1000, "hello window")
	rl.SetTargetFPS(61)
	for !rl.WindowShouldClose() {
		rl.BeginDrawing()
		rl.ClearBackground(rl.BLACK)
		rl.DrawText("hello world!", 400, 480, 20, rl.PINK)
		rl.EndDrawing()

	}
}
