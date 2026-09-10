#include "utils.hpp"



int main(){
	LStr str = *BStr::format("henlo:%d", 3);
	str += " :";
	str += LStr(3);
	for(size_t i =0; i<600; i++){
		str += LStr(i);
	}
	printf("%s\n", *str);
}	

