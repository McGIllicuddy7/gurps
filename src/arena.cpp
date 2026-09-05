#include "arena.h"

void * Arena::malloc(size_t byte_count){
	if(m_offset%16 != 0){
		m_offset += 16-m_offset%16;
	}
	if(m_offset+byte_count>=m_byte_count){
		if(m_next){
			return m_next->malloc(byte_count);
		}else{
			m_next = std::unique_ptr<Arena>(new Arena(m_byte_count*2));
			return m_next->malloc(byte_count);
		}
	}
	uint8_t * output = m_bytes.get()+m_offset;
	m_offset += byte_count;
	return output;
}

Arena::Arena(){
	m_bytes = std::unique_ptr<uint8_t[]>(new uint8_t[4096*4]);
	m_offset =0;
	m_byte_count = 4096*4;
}

Arena::Arena(size_t byte_count){
	m_bytes = std::unique_ptr<uint8_t[]>(new uint8_t[byte_count]);
	m_offset =0;
	m_byte_count = byte_count;
}
void Arena::defer(std::function<void()>&&func ){
	m_defered.push_back(func);
}


Arena::~Arena(){
	if(m_defered.size()>0){
		int32_t count = m_defered.size()-1;
		for(int32_t i = count; i>=0; i--){
			(m_defered[i])();
		}
	}
}
