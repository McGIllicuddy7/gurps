#pragma once
#include "utils.h"
#include <string.h>
#include <type_traits>
#include <typeinfo>
class BiteStream;

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
	template<typename T> void serialize(const T& value){
		frd_serialize(*this, value);
	}
	template<typename T> void deserialize(T& value){
		frd_deserialize(*this, value);
	}
};



class DeserializationOutofBytes:public std::exception{};

class DeserializationWrongSize:public std::exception{};

template<typename T> void frd_serialize(BiteStream & stream, const T& value){
	static_assert(std::is_trivially_copyable<T>());
	static_assert(std::is_trivially_destructible<T>());
	uint32_t sz = sizeof(T);
	stream.write_bytes((uint8_t*)&sz, sizeof(sz));
	if constexpr(std::is_pointer<T>()){
		if(value){
			stream.write_byte(1);
			frd_serialize(stream, *value);
		}else{
			stream.write_byte(0);
		}
	}else{
		stream.write_bytes((const uint8_t*)&value, sizeof(value));
	}
}

template<typename T> void frd_deserialize(BiteStream & stream, T& out_param){
	static_assert(std::is_trivially_copyable<T>());
	static_assert(std::is_trivially_destructible<T>());
	uint32_t sz;
	if(!(stream.read_bytes((uint8_t*)&sz, sizeof(sz)))){
		throw DeserializationWrongSize{};
	}
	if constexpr(std::is_pointer<T>()){
		uint8_t value;
		assert(stream.read_byte(value));
		if(value){
			out_param = new typename std::remove_pointer<T>::type ();
			frd_deserialize(stream, *out_param);
		}else{
			out_param = nullptr;
		}
	}else{
		uint8_t output[sizeof(T)];
		if(!(stream.read_bytes(output, sizeof(T)))){
			throw DeserializationOutofBytes{};
		}
		out_param = *(T*)(output);
	}
}

template<typename T> void frd_serialize(BiteStream & stream, const vector<T>& list){
	uint32_t count = list.size();
	frd_serialize(stream, count);
	for(size_t i =0; i<list.size(); i++){
		frd_serialize(stream, list[i]);
	}
}
template<typename T> void frd_deserialize(BiteStream & stream, vector<T>& list){
	uint32_t count;
	list.clear();
	frd_deserialize(stream, count);
	for(size_t i =0; i<count; i++){
		T tmp;
		frd_deserialize(stream, tmp);
		list.push_back(std::move(tmp));
	}
}

template<> inline void frd_serialize(BiteStream&stream, const std::string& str){
	uint32_t count = str.size();
	frd_serialize(stream, count);
	stream.write_bytes((uint8_t*)&str[0], count);
}

template<> inline void frd_deserialize(BiteStream& stream, std::string& str){
	uint32_t count;
	frd_deserialize(stream, count);
	str.clear();
	for(size_t i =0; i<count; i++){
		str.push_back('a');
	}
	if(!stream.read_bytes((uint8_t*)&str[0], count)){
		throw DeserializationOutofBytes{};
	}
}

#define MAKE_SERIALIZABLE(T,stream, value,EXPR)\
template<> inline void frd_serialize(BiteStream& stream, const T& value) EXPR

#define MAKE_DESERIALIZABLE(T,stream, out_param,EXPR)\
template<> inline void frd_deserialize(BiteStream& stream, T& out_param) EXPR


template<typename T> void frd_serialize(BiteStream & stream, const std::unique_ptr<T>& ptr){
	frd_serialize(stream, ptr.get());
}

template<typename T> void frd_deserialize(BiteStream & stream, std::unique_ptr<T>& ptr){
	T* ptr_ptr = ptr.get();
	frd_deserialize(stream, ptr_ptr);
	ptr = unique_ptr<T>(ptr_ptr);
}