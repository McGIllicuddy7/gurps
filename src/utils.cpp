#include "utils.hpp"
void BiteStream::write_byte(u8 byte){
    m_bytes.push_back(byte);
}

bool BiteStream::read_byte(u8& byte){
    if(m_next>= m_bytes.size()){
        return false;
    }
    byte = m_bytes[m_next];
    m_next+=1;
    return true;
}
void BiteStream::write_bytes(const u8* ptr, size_t count){
    for(size_t i =0; i<count; i++){
        this->write_byte(ptr[i]);
    }
}

bool BiteStream::read_bytes(u8* ptr, size_t count){
    for(size_t i =0; i<count; i++){
        if(!this->read_byte(*(ptr+i))){
            return false;
        }
    }
    return true;
}

vector<u8>& BiteStream::get_bytes(){
    return m_bytes;
}

const vector<u8>& BiteStream::get_bytes() const {
    return m_bytes;
}

BiteStream BiteStream::from_bytes(vector<u8> bytes){
    BiteStream out;
    out.m_bytes = bytes;
    out.m_next =0;
    return out;
}


vector<u8> read_file_to_bytes(const string& file_name){
    FILE * f = fopen(file_name.c_str(), "rb");
    if(!f){
        return {};
    }
    fseek(f, 0, SEEK_END);
    size_t count = ftell(f);
    fseek(f, 0, SEEK_SET);
    vector<u8> out;
    out.reserve(count);
    for(size_t i =0; i<count; i++){
        out.push_back(0);
    }
    fread(&out[0], 1, count, f);
    fclose(f);
    return out;
}

void write_bytes_to_file(const string& file_name, const vector<u8>& bites){
    FILE * f = fopen(file_name.c_str(), "wb");
    if(!f){
        return;
    }
    fwrite(&bites[0], 1, bites.size(), f);
    fclose(f);
}

void BiteStream::write_to_file(const string& path)const {
    write_bytes_to_file(path, m_bytes);
}
BiteStream BiteStream::read_from_file(const string&path){
    return BiteStream::from_bytes(read_file_to_bytes(path));
}