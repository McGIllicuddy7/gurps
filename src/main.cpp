//#include "game.h"
//#include "gui.h"
//#include "serializer.h"
//#include "utils.h"
#include "arena.h"

struct tester{
	int32_t v;
	~tester(){
		printf("destroyed:%d\n", v);
	}	
};
int main() {
	Arena arena = Arena();
	for(size_t _i =0; _i<100; _i++){
	std::vector<tester> * list = arena.alloc<std::vector<tester >>({});
	for(size_t i =0; i<100; i++){
		list->push_back({(int32_t)i});
	}
	for(size_t i =0; i<list->size(); i++){
		printf("%d\n", (*list)[i].v);
	}
	}
}

