#include "utils.hpp"

int main(){
	bool should_serialize = false;
	srand(time(0));
	if(should_serialize){
		BiteStream bites;
		vector<vector<unique_ptr<string>>> s;
		for(i32 i =0; i<10; i++){
			vector<unique_ptr<string>> tmp ;
			for(i32 j = 0; j<=i; j++){
				if(rand()%10 >0){
					char buff[128] = {0};
					snprintf(buff, 127, "%d",j);
					tmp.push_back(std::move(make_unique<string>(buff)));
				}else{
					tmp.push_back({});
				}
			}
			s.push_back(std::move(tmp));	
		}
		bites.serialize(s);
		bites.write_to_file("test.txt");
	}else{
		BiteStream bites = BiteStream::read_from_file("test.txt");
		vector<vector<unique_ptr<string>>> s;
		assert(bites.deserialize(s));
		for(size_t i =0; i<s.size(); i++){
			for(size_t j =0; j<s[i].size(); j++){
				if(s[i][j]){
					printf("i:%zu | %s\n", i,s[i][j]->c_str());
				}else{
					printf("i%zu | null\n", i);
				}
			}
		}
	}

}