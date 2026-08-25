#include "utils.h"
#include "string.h"

str::str() {}
str::str(const char* st) {
    size_t len = strlen(st);
    m_ptr = std::make_shared<char[]>(len + 1);
    memcpy(m_ptr.get(), st, len);
    m_ptr[len] = 0;
}
str::str(string_view st) {
    size_t len = st.size();
    m_ptr = std::make_shared<char[]>(len + 1);
    memcpy(m_ptr.get(), &st[0], len);
    m_ptr[len] = 0;
}
str::str(const string& st) {
    size_t len = st.size();
    m_ptr = std::make_shared<char[]>(len + 1);
    memcpy(m_ptr.get(), &st[0], len);
    m_ptr[len] = 0;
}
str::operator string_view() {
    string_view out = (char*)*this;
    return out;
}

str::operator const char* () const {
    return m_ptr.get();
}
str::operator char* () {
    return m_ptr.get();
}

const char* str::const_ptr() const {
    return m_ptr.get();
}

str str::operator+(string_view base) const {
    size_t sz = this->size();
    size_t l = sz + base.size();
    shared_ptr<char[]> ptr = make_shared<char[]>(l + 1);
    memcpy(ptr.get(), this->const_ptr(), sz);
    memcpy(ptr.get() + sz, &base[0], base.size());
    ptr[l] = 0;
    str out;
    out.m_ptr = ptr;
    return out;

}
const char& str::operator[](size_t idx)const {
    return m_ptr[idx];
}
char& str::operator[](size_t idx) {
    return m_ptr[idx];
}

size_t str::size()const {
    if (!this->m_ptr) {
        return 0;
    }
    return strlen((const char*)*this);
}
char* str::begin() {
    return m_ptr.get();
}
char* str::end() {
    return m_ptr.get() + this->size();
}

char* str::ptr() {
    return m_ptr.get();
}

const char* str::begin() const {
    return m_ptr.get() + this->size();
}
const char* str::end() const {
    return m_ptr.get() + this->size();
}
string_view str::sub_string(size_t start, size_t end)const {
    return string_view(m_ptr.get() + start, end);
}
str str::clone() const {
    return str(this->const_ptr());
}
