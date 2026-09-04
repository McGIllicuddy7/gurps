#include "game.h"
#include "gui.h"
#include "serializer.h"
#include "utils.h"
using std::make_unique;
#include <stdio.h> 
struct Person{
	string name;
	int32_t age;	
};

MAKE_SERIALIZABLE(Person, stream, value,{
	stream.serialize(value.name); 
	stream.serialize(value.age);
});

MAKE_DESERIALIZABLE(Person, stream, value,{
	stream.deserialize(value.name); 
	stream.deserialize(value.age);
});

int main() {
	srand(time(0));
	bool should_serialize = false;
	if (should_serialize){
		vector<unique_ptr<Person>> people;
		for(int32_t i =0; i<1000; i++){
			Person p{str_format("henlo: %d", i), i*3+2};
			if(random()%4 !=0){
				people.push_back(std::move(make_unique<Person>(Person(p))));
			}else{
				people.push_back(unique_ptr<Person>());
			}
		}
		BiteStream c;
		c.serialize(people);
		write_bytes_to_file("test.txt",c.get_data());
	}else{
		BiteStream bites = BiteStream::from_file(read_file_to_bytes("test.txt"));
		vector<unique_ptr<Person>> people;
		bites.deserialize(people);
		for(size_t i =0; i<people.size(); i++){
			if(people[i]){
				printf("{%s, %d}\n", people[i]->name.c_str(), people[i]->age);
			}else{
				printf("null\n");
			}
		}	
	}
}
