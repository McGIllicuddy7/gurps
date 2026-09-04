#pragma once
#include "utils.h"
#include <string.h>
#include <type_traits>
#include <typeinfo>
class BiteStream{
	vector<uint8_t> m_bites;
	size_t m_ptr = 0;
public:
	static BiteStream from_file(vector<uint8_t>);
	void write_byte(uint8_t v);
	bool read_byte(uint8_t& output);
	void write_bytes(const uint8_t * v, size_t size);
	bool read_bytes(uint8_t *output, size_t size);	
	vector<uint8_t>& get_data();
	const vector<uint8_t>& get_data() const;
};


template<typename T>  inline const char * typename_of(){
	static_assert(false);
}
 template<>  inline const char *typename_of<int32_t>(){
	return "int32_t";
} 

template<typename T> void serialize(BiteStream & stream, const T& value){
	static_assert(std::is_trivially_copyable<T>());
	const char * sz = typename_of<T>();
	printf("name of type:%s\n", sz);
	stream.write_bytes((const uint8_t*)sz, strlen(sz)+1);
	if constexpr(std::is_pointer<T>()){
		if(value){
			stream.write_byte(1);
			serialize(stream, *value);
		}else{
			stream.write_byte(0);
		}
	}else{
		stream.write_bytes((const uint8_t*)&value, sizeof(value));
	}
}

template<typename T> T deserialize(BiteStream & stream){
	static_assert(std::is_trivially_copyable<T>());
	
	const char * sz = typename_of<T>();
	do{
		uint8_t x;
		assert(stream.read_byte(x));
		assert(x ==*sz); 
	}while(*(sz++));	
	if constexpr(std::is_pointer<T>()){
		uint8_t value;
		assert(stream.read_byte(value));
		if(value){
			return new std::remove_pointer<T> (deserialize<std::remove_pointer<T>>);
		}else{
			return nullptr;
		}
	}else{
		uint8_t output[sizeof(T)];
		assert(stream.read_bytes(output, sizeof(T)));
		return *(T*)(output);
	}

}


