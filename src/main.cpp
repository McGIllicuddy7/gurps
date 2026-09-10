#include "utils.hpp"
#include "engine.hpp"
struct Test{
	string name;
	int32_t x;
	int32_t y;
	int32_t health;
};

G_MAKE_SERIALIZEABLE(Test, G_FIELD(name), G_FIELD(x), G_FIELD(y), G_FIELD(health));

int main(){
	bool serialize = false;
	if (serialize){
		BiteStream bites;
		Test test;
		test.name = "bridget :3";
		test.x = 420;
		test.y = 69;
		test.health = 67;
		bites.serialize(test);
		bites.write_to_file("test.txt");
	}else{
		BiteStream bites = BiteStream::read_from_file("test.txt");
		Test test;
		assert(bites.deserialize(test));
		printf("Test{name:%s, x:%d, y:%d, health:%d}\n", test.name.c_str(), test.x, test.y, test.health);
	}
}	

void game_runtime_setup(){}
