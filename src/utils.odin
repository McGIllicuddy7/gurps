package main
import "base:runtime"
import "core:fmt"
import "core:strings"
rt :: runtime
strings :: strings
import "core:mem"
debug_main :: proc(to_run: proc()) {
	track: mem.Tracking_Allocator
	mem.tracking_allocator_init(&track, context.allocator)
	defer mem.tracking_allocator_destroy(&track)
	context.allocator = mem.tracking_allocator(&track)
	to_run()
	for _, leak in track.allocation_map {
		fmt.printf("%v leaked %m\n", leak.location, leak.size)
	}
}
