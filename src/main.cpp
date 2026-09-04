#include "game.h"
#include "gui.h"
#include "serializer.h"
#include "utils.h"
#include <stdio.h> 
int main() {
	bool should_serialize =false;
	if (should_serialize){
		BiteStream c;
		vector<string> s;
		for(size_t i =0; i<100; i++){
			char buff[100] = {0};
			snprintf(buff, 99, "%d", (int32_t)i);
			s.push_back(buff);
		}
		serialize(c, s);
		write_bytes_to_file("test.txt", c.get_data());
		printf("byte count:%zu\n", c.get_data().size());
	}else{
		BiteStream bites = BiteStream::from_file(read_file_to_bytes("test.txt"));
		vector<string> strs;
		deserialize(bites, strs);
		for(size_t i =0; i<strs.size(); i++){
			printf("%s\n" ,strs[i].c_str());
		}
		printf("byte count:%zu\n", bites.get_data().size());
	}
}
