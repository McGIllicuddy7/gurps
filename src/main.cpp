#include "utils.hpp"



int main(){
	BStr str = BStr::format("henlo:%d", 3);
	str += " :";
	str += BStr(3);
	for(size_t i =0; i<500; i++){
		str += BStr(i);
	}
	printf("%s\n", *str);
}	

