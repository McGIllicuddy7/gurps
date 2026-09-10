#pragma once
#include <string>
#include <vector>
#include <array>
#include <functional>
#include <type_traits>
#include <memory>
#include <assert.h>
#include <stdio.h>
using std::vector;
using std::array;
using std::string;
using std::function;
using std::unique_ptr;
using std::make_unique;

template<typename T> class Slice{
    T* m_ptr = 0;
    size_t m_count = 0;
    public:
    size_t size()const {
        return m_count;
    }

    T* begin(){
        return m_ptr;
    }

    T* end(){
        if(m_ptr){
            return m_ptr+m_count;
        }else{
            return nullptr;
        }
    }

    const T* begin()const {
        return m_ptr;
    }

    const T* end()const {
        if(m_ptr){
            return m_ptr+m_count;
        }else{
            return nullptr;
        }
    } 

    const T& operator[](size_t idx) const{
        assert(idx< m_count);
        assert(m_ptr);
        return m_ptr[idx];
    }

    T& operator[](size_t idx) {
        assert(idx< m_count);
        assert(m_ptr);
        return m_ptr[idx];
    }
    Slice(T* ptr, size_t count){ 
        m_ptr = ptr;
        m_count =count;
    }

    Slice(vector<T>& list){
        if(list.size() >0){
            m_ptr = &list[0];
            m_count = list.size();
        }else{
            m_ptr =0;
            m_count =0;
        }
    }

    template<size_t count> Slice(array<T, count>& array){
        if(array.size()>0){
            m_ptr = &array[0];
            m_count = count;
        }else{
            m_ptr = 0;
            m_count =0;
        }
    }
};

template<typename T> class OwnedSlice{
    unique_ptr<T[]> m_ptr;
    size_t m_count =0;
    public:
    size_t size()const {
        return m_count;
    }

    T* begin(){
        return m_ptr.get();
    }

    T* end(){
        if(m_ptr){
            return m_ptr.get()+m_count;
        }else{
            return nullptr;
        }
    }

    const T* begin()const {
        return m_ptr.get();
    }

    const T* end()const {
        if(m_ptr){
            return m_ptr.get()+m_count;
        }else{
            return nullptr;
        }
    } 

    const T& operator[](size_t idx) const{
        assert(idx< m_count);
        assert(m_ptr);
        return m_ptr[idx];
    }

    T& operator[](size_t idx) {
        assert(idx< m_count);
        assert(m_ptr);
        return m_ptr[idx];
    }
    OwnedSlice(){
        m_ptr =unique_ptr<T[]>(0);
        m_count =0;
    }
    OwnedSlice(unique_ptr<T[]>&& ptr, size_t count){
        m_ptr = std::move(ptr);
        m_count = count;
    }  

    static OwnedSlice<T> create(size_t count){
        unique_ptr<T[]> out = make_unique<T[]>(count);
        for(size_t i =0; i<count; i++){
            new (&out[i]) T();
        }
        return OwnedSlice<T>(std::move(out), count);
    }
    static OwnedSlice<T> unsafe_create_uninit(size_t count){
        unique_ptr<T[]> out = make_unique<T[]>(count);
        return OwnedSlice<T>(std::move(out), count);
    } 
    operator Slice<T> (){
        return Slice<T>(m_ptr.get(), m_count);
    }

};

template<typename T> class SharedSlice{
    std::shared_ptr<T[]> m_ptr;
    size_t m_count =0;
    public:
    size_t size()const {
        return m_count;
    }

    T* begin(){
        return m_ptr.get();
    }

    T* end(){
        if(m_ptr){
            return m_ptr.get()+m_count;
        }else{
            return nullptr;
        }
    }

    const T* begin()const {
        return m_ptr.get();
    }

    const T* end()const {
        if(m_ptr){
            return m_ptr.get()+m_count;
        }else{
            return nullptr;
        }
    } 

    const T& operator[](size_t idx) const{
        assert(idx< m_count);
        assert(m_ptr);
        return m_ptr[idx];
    }

    T& operator[](size_t idx) {
        assert(idx< m_count);
        assert(m_ptr);
        return m_ptr[idx];
    }
    SharedSlice(){
        m_ptr = std::shared_ptr<T[]>(0);
        m_count =0;
    }

    SharedSlice(std::shared_ptr<T[]>&& ptr, size_t count){
        m_ptr = std::move(ptr);
        m_count = count;
    }

    static SharedSlice<T> create(size_t count){
        std::shared_ptr<T[]> out = std::shared_ptr<T[]>(new T[count]);
        for(size_t i =0; i<count; i++){
            new (&out[i]) T();
        }
        return SharedSlice<T>(std::move(out), count); 
    }

    static SharedSlice<T> unsafe_create_uninit(size_t count){
        std::shared_ptr<T[]> out = std::shared_ptr<T[]>(new T[count]);
        return SharedSlice<T>(std::move(out), count); 
    }
    operator Slice<T> (){
        return Slice<T>(m_ptr.get(), m_count);
    }

};

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using usize = size_t;
using isize = ssize_t;

class BiteStream{
    vector<u8> m_bytes;
    size_t m_next =0;
    public:
    void write_byte(u8 byte);
    //returns false on error
    [[nodiscard]] bool read_byte(u8& byte);
    void write_bytes(const u8* ptr, size_t count);
    //returns false on error
    [[nodiscard]] bool read_bytes(u8* ptr, size_t count);
    template<typename T> void serialize(const T& value){
        frd_serialize(*this, value);
    }
    template<typename T> [[nodiscard]] bool deserialize(T& value){
        return frd_deserialize(*this, value);
    }
    vector<u8>& get_bytes();
    const vector<u8>& get_bytes() const ;
    static BiteStream from_bytes(vector<u8> bytes);
    void write_to_file(const string& path)const;
    static BiteStream read_from_file(const string & path);
};



template<typename T> void frd_serialize(BiteStream& stream,const T& v){
    static_assert(std::is_pod<T>(), "must be plain old data");
    if constexpr (std::is_pointer<T>()){
        if constexpr (std::is_same<T, char*>()){
                if(v){
                    u32 count = strlen(v);
                    stream.write_bytes((u8*)&count, sizeof(count));
                    stream.write_bytes((u8*)v, count);
                }else{
                    u32 count = 0;
                    stream.write_bytes((u8*)&count, sizeof(count));
                }
        }else{
            if(v){
                u32 size =1;
                stream.write_bytes((u8*)&size, sizeof(u32));
                stream.write_byte(1);
                frd_serialize(stream, *v);
            }else{
                u32 size =1;
                stream.write_bytes((u8*)&size, sizeof(u32));
                stream.write_byte(0);
            }
        }
    }else{
        u32 size = sizeof(T);
        stream.write_bytes((u8*)&size, sizeof(size));
        stream.write_bytes((const u8*)&v, sizeof(T));
    }
}

//returns false on error
template<typename T>  [[nodiscard]] bool frd_deserialize(BiteStream& stream,T& v){
    static_assert(std::is_pod<T>(), "must be plain old data");
    u32 size = -1;
    if(!stream.read_bytes((u8*)&size, sizeof(size))){
        return false;
    }
    if constexpr (std::is_pointer<T>()){
        if constexpr(std::is_same<T, char *>()){
            u8 is_null = (size == 0);
            if(is_null){
                v =0;
                return true;
            }
            v = (char*)malloc(sizeof(size+1));
            v[size] =0;
            if(!stream.read_bytes((u8*)&v[0],size)){
                return false;
            }
            return true;
        }else{
            if(size != 1){
                return false;
            }
            u8 bl;
            if(!stream.read_byte(bl)){
                return false;
            }
            if (bl){
                T out = new typename std::remove_pointer<T>::type ();
                if(!frd_deserialize(stream, *out)){
                    delete out;
                    return false;
                }
                v = out;
            }else{
                v = nullptr;
            }
        }
    }else{
        u_int8_t output[sizeof(T)];
        if(size != sizeof(T)){
            printf("size:%u, sizeof(T): %zu\n",size, sizeof(T));
            assert(false);
            return false;
        }
        if(!stream.read_bytes(output, sizeof(T))){
            return false;
        }
        v = *(T*)output;
    }
    return true;
}

template<> inline void frd_serialize(BiteStream & stream, const string& v){
    u32 count = v.size();
    stream.write_bytes((u8*)&count, sizeof(count));
    stream.write_bytes((u8*)&v[0], v.size());
}

template<>  [[nodiscard]] inline bool frd_deserialize(BiteStream & stream, string& v){
    u32 count;
    if(!stream.read_bytes((u8*)&count, sizeof(count)))  {
        return false;
    }
    v.clear();
    for(u32 i =0; i<count;i++){
        v.push_back('x');
    }
    if(!stream.read_bytes((u8*)&v[0],count)){
        return false;
    }
    return true;
}

template<typename T> void frd_serialize(BiteStream& stream, const vector<T>& list){
    frd_serialize(stream, (u32)list.size());
    for(size_t i =0; i<list.size(); i++){
        frd_serialize(stream, list[i]);
    }
}

template<typename T> [[nodiscard]] bool frd_deserialize(BiteStream& stream, vector<T>& list){
    u32 count;
    list.clear();
    if(!frd_deserialize(stream, count)){
        return false;
    } 
    for(size_t i =0; i<count; i++){
        T output;
        if(!frd_deserialize(stream, output)){
            return false;
        }
        list.push_back(std::move(output));
    }
    return true;
}

template<typename T> void frd_serialize(BiteStream& stream,const  unique_ptr<T>& value){
    bool is_valid = bool(value);
    frd_serialize(stream,is_valid);
    if(value){
        frd_serialize(stream, *value.get());
    }
}
template<typename T> [[nodiscard]]bool frd_deserialize(BiteStream& stream, unique_ptr<T>& value){
    bool is_nonnull;
    value = unique_ptr<T>(nullptr);
    if(!frd_deserialize(stream, is_nonnull)){
        return false;
    }
    if(is_nonnull){
        T v;
        if(!frd_deserialize(stream, v)){
            return false;
        }
        value = make_unique<T>(std::move(v));
    }
    return true;
}

template<typename T, size_t count> void frd_serialize(BiteStream& stream, const array<T,count>& list){
    frd_serialize(stream, (u32)list.size());
    for(size_t i =0; i<list.size(); i++){
        frd_serialize(stream, list[i]);
    }
}

template<typename T, size_t count> [[nodiscard]] bool frd_deserialize(BiteStream& stream,array<T, count>& list){
    u32 icount;
    if(!frd_deserialize(stream, icount)){
        return false;
    } 
    if(icount !=count){
        return false;
    }
    for(size_t i =0; i<count; i++){
        T output;
        if(!frd_deserialize(stream, output)){
            return false;
        }
        list[i] =std::move(output);
    }
    return true;
}

vector<u8> read_file_to_bytes(const string& file_name);
void write_bytes_to_file(const string& file_name, const vector<u8>& bites);


class Arena{
    unique_ptr<u8[]> m_bytes;
    size_t m_capacity =0;
    size_t m_next =0;
    unique_ptr<Arena> m_next_ptr;
    vector<function<void()>> m_destructor_queue;
    public:
    Arena();
    Arena(size_t cap);
    ~Arena();
    void * alloc(size_t count); 
    void defer(function<void()> to_defer);
    template<typename T>T* create(T&& value){
        T* out = (T*)this->alloc(sizeof(T));
        new (out)T(value);
        if constexpr(!std::is_trivially_destructible<T>()){
            this->defer([=]{
                out->~T();
            });
        }
        return out;
    }

    template<typename T>T* create(const T& value){
        T* out = (T*)this->alloc(sizeof(T));
        new (out)T(value);
        if constexpr(!std::is_trivially_destructible<T>()){
            this->defer([=]{
                out->~T();
            });
        }
        return out;
    }

    template<typename T>T* create(){
        T* out = (T*)this->alloc(sizeof(T));
        new (out)T();
        if constexpr(!std::is_trivially_destructible<T>()){
            this->defer([=]{
                out->~T();
            });
        }
        return out;
    }
    template<typename T> Slice<T> create_slice(size_t count){
        T* out = (T*)this->alloc(sizeof(T)*count);
        for(size_t i =0; i<count; i++){
            new (out+i)T();
        }
        return Slice<T>(out, count);
    }

    template<typename T> Slice<T> unsafe_create_slice_uninit(size_t count){
        T* out = (T*)this->alloc(sizeof(T)*count);
        return Slice<T>(out, count);
    }
    void reset();
};

char * fmt_string(Arena * arena, const char * fmt, ...);
extern Arena tmp_arena;


template<typename...T> void frd_serialize_fields(BiteStream & stream, const T&...fields){
    ((void)stream.serialize(fields),...);
}


template<typename Ty> void frd_fuckery(BiteStream & stream, bool& outparam, Ty& field){
    if(!stream.deserialize(field)){
        outparam = false;
    }
};
template<typename...T> bool frd_deserialize_fields(BiteStream & stream, T&...fields){
    bool out = true;
    ((void)frd_fuckery(stream, out, fields), ...);
    return out;
}

#define G_FIELD(field) (k_struct.field)
#define G_MAKE_SERIALIZEABLE(Type,ARGS...) \
template<> inline void frd_serialize(BiteStream& stream, const Type& k_struct){\
    frd_serialize_fields(stream, ARGS);\
}\
template<> [[nodiscard]]inline bool frd_deserialize(BiteStream& stream, Type& k_struct){\
    return frd_deserialize_fields(stream, ARGS);\
}\


