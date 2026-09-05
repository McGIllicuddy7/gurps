#pragma once
#include <stdlib.h>
#include <functional>
#include <vector>
#include <memory>
class Arena{
	std::unique_ptr<uint8_t[]> m_bytes;
	size_t m_offset;
	size_t m_byte_count;	
	std::vector<std::function<void()>> m_defered;
	std::unique_ptr<Arena> m_next;
public:
	Arena();
	Arena(size_t count);
	void * malloc(size_t byte_count);
	~Arena();
	template<typename T> T* alloc(T&& value){
		T* out = (T*)this->malloc(sizeof(T));
		new (out) T(value);
		if constexpr(!std::is_trivially_destructible<T>()){
			m_defered.push_back([=]{
				out->~T();	
			});
		}
		return out;
	}
	void defer(std::function<void()> &&func);
};
