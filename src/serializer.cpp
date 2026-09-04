#include "serializer.h"

void BiteStream::write_byte(uint8_t v){
	m_bites.push_back(v);
}

bool BiteStream::read_byte(uint8_t&output){
	if(m_ptr >= m_bites.size()){
		return false;
	}else{
		output = m_bites[m_ptr];
		m_ptr++;
	}
	return true;
}

void BiteStream::write_bytes(const uint8_t * v, size_t size){
	for(size_t i =0; i<size; i++){
		m_bites.push_back(v[i]);
	}
}

bool BiteStream::read_bytes(uint8_t * output, size_t size){
	for(size_t i =0; i<size; i++){
		if(!this->read_byte(*(output+i))){
			return false;
		}
	}
	return true;
}



vector<uint8_t>& BiteStream::get_data(){
	return m_bites;
}

const vector<uint8_t>& BiteStream::get_data() const{
	return m_bites;
}


BiteStream BiteStream::from_file(vector<uint8_t> bites){
	BiteStream out;
	out.m_ptr =0;
	out.m_bites = bites;
	return out;

}