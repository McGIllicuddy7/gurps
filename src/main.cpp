#include "utils.hpp"

int main(){
	Arena arena;
	Slice<string> v = arena.create_slice<string>(100);
	for(i32 i =0; i<v.size(); i++){
		v[i]= fmt_string(&arena, "henlo:%d", i);
	}
	for(const auto &i: v){
		printf("%s\n", i.c_str());
	}
}