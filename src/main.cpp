#include "game.h"
#include "gui.h"
#include "serializer.h"
#include "utils.h"
#include <stdio.h> 
int main() {
	bool should_serialize = false;
	if (should_serialize){
		BiteStream c;
		serialize(c, 10);
		write_bytes_to_file("test.txt", c.get_data());
		printf("byte count:%zu\n", c.get_data().size());
	}else{
		BiteStream bites = BiteStream::from_file(read_file_to_bytes("test.txt"));
		int32_t x = deserialize<int32_t>(bites);
		printf("%d\n",x);
		printf("byte count:%zu\n", bites.get_data().size());
	}
}
