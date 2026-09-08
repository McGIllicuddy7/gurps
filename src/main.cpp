#include "utils.hpp"

int main(){
	bool should_serialize = false;
	srand(time(0));
	if(should_serialize){
		BiteStream bites;
		vector<vector<char*>> s;
		for(i32 i =0; i<500; i++){
			vector<char*> tmp ;
			for(i32 j = 0; j<=i; j++){
				if(rand()%10 >0){
					char * p= (char*)malloc(128);
					snprintf(p,126,"%d", j);
					tmp.push_back(p);
				}else{
					tmp.push_back(0);
				}
			}
			s.push_back(std::move(tmp));	
		} 
		bites.serialize(s);
		bites.write_to_file("test.txt");
	}else{
		BiteStream bites = BiteStream::read_from_file("test.txt");
		vector<vector<char*>> s;
		assert(bites.deserialize(s));
		for(size_t i =0; i<s.size(); i++){
			for(size_t j =0; j<s[i].size(); j++){
				if(s[i][j]){
					printf("i:%zu | %s\n", i,s[i][j]);
				}else{
					printf("i%zu | null\n", i);
				}
			}
		}
	}

}