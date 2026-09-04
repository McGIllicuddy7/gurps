#include "game.h"
#include "gui.h"
#include "serializer.h"
#include "utils.h"
#include <stdio.h> 
struct FuckYou{
	string msg;
	size_t idx;
};
template<> inline void frd_serialize(BiteStream & stream, const FuckYou & fuck){
	stream.serialize(fuck.msg);
	stream.serialize(fuck.idx);
}

template<> inline void frd_deserialize(BiteStream & stream, FuckYou & fuck){
	stream.deserialize(fuck.msg);
	stream.deserialize(fuck.idx);
}

int main() {
	bool should_serialize =false;

	if (should_serialize){
		BiteStream c;
		FuckYou fuck,u;
		fuck.idx = 420;
		fuck.msg = "god hates fags lmaoooooo";
		u.idx = 69;
		u.msg = "google image search frotting";
		c.serialize(fuck);
		c.serialize(u);
		write_bytes_to_file("test.txt",c.get_data());
	}else{
		BiteStream bites = BiteStream::from_file(read_file_to_bytes("test.txt"));
		vector<string> strs;
		FuckYou fuck,u;
		bites.deserialize(fuck);
		bites.deserialize(u);
		printf("%zu,%s\n", fuck.idx, fuck.msg.c_str());
		printf("%zu,%s\n", u.idx, u.msg.c_str());
	}
}
