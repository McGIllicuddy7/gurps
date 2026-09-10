#include "utils.hpp"

Result<i32> is_even(i32 x){
	if(x%2 == 0){
		return {x};
	}else{
		return Result<i32>::Err("is error");
	}
}

int main(){
	for(i32 i =0; i<15; i++){
		Result<i32> x = is_even(i);
		if(x.is_err()){
			printf("%s\n", x.message());
		}else{
			printf("%d\n", x.get());
		}
	}
}	

