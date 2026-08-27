#include "utils.h"
#include "string.h"
extern shared_ptr<char[]> alloc_str(size_t len);
str::str() {}
str::str(const char* st) {
    size_t len = strlen(st);
    m_ptr = alloc_str(len);
    memcpy(m_ptr.get(), st, len);
    m_ptr[len] = 0;
    m_len = len;
}
str::str(string_view st) {
    size_t len = st.size();
    m_ptr = alloc_str(len);
    memcpy(m_ptr.get(), &st[0], len);
    m_ptr[len] = 0;
    m_len = len;
}
str::str(const string& st) {
    size_t len = st.size();
    m_ptr = alloc_str(len);
    memcpy(m_ptr.get(), &st[0], len);
    m_ptr[len] = 0;
    m_len = len;
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
    shared_ptr<char[]>ptr = alloc_str(l);
    memcpy(ptr.get(), this->const_ptr(), sz);
    memcpy(ptr.get() + sz, &base[0], base.size());
    ptr[l] = 0;
    str out;
    out.m_ptr = ptr;
    out.m_len = l;
    return out;
}

void str::operator+= (string_view base) {
    *this = *this + base;
}

const char& str::operator[](size_t idx)const {
    assert(idx < this->size());
    return m_ptr[idx];
}
char& str::operator[](size_t idx) {
    assert(idx < this->size());
    return m_ptr[idx];
}

size_t str::size()const {
    return m_len;
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

str::str(int32_t x) {
    char st[128] = { 0 };
    snprintf(st, 127, "%d", x);
    size_t len = strlen(st);
    m_ptr = alloc_str(len);
    memcpy(m_ptr.get(), st, len);
    m_ptr[len] = 0;
    m_len = len;
}

str::str(float x) {
    char st[128] = { 0 };
    snprintf(st, 127, "%f", x);
    size_t len = strlen(st);
    m_ptr = alloc_str(len);
    memcpy(m_ptr.get(), st, len);
    m_ptr[len] = 0;
    m_len = len;
}

str::str(char c) {
    char st[128] = { 0 };
    snprintf(st, 127, "%c", c);
    size_t len = strlen(st);
    m_ptr = alloc_str(len);
    memcpy(m_ptr.get(), st, len);
    m_ptr[len] = 0;
    m_len = len;
}

str::str(uint32_t x) {
    char st[128] = { 0 };
    snprintf(st, 127, "%u", x);
    size_t len = strlen(st);
    m_ptr = alloc_str(len);
    memcpy(m_ptr.get(), st, len);
    m_ptr[len] = 0;
    m_len = len;
}
str::str(uint64_t x) {
    char st[128] = { 0 };
    snprintf(st, 127, "%llu", x);
    size_t len = strlen(st);
    m_ptr = alloc_str(len);
    memcpy(m_ptr.get(), st, len);
    m_ptr[len] = 0;
    m_len = len;
}

str::str(uint16_t x) {
    char st[128] = { 0 };
    snprintf(st, 127, "%u", (uint32_t)x);
    size_t len = strlen(st);
    m_ptr = alloc_str(len);
    memcpy(m_ptr.get(), st, len);
    m_ptr[len] = 0;
    m_len = len;
}

str::str(uint8_t x) {
    char st[128] = { 0 };
    snprintf(st, 127, "%u", (uint32_t)x);
    size_t len = strlen(st);
    m_ptr = alloc_str(len);
    memcpy(m_ptr.get(), st, len);
    m_ptr[len] = 0;
    m_len = len;
}

str::str(int64_t x) {
    char st[128] = { 0 };
    snprintf(st, 127, "%lld", x);
    size_t len = strlen(st);
    m_ptr = alloc_str(len);
    memcpy(m_ptr.get(), st, len);
    m_ptr[len] = 0;
    m_len = len;
}

str::str(int16_t x) {
    char st[128] = { 0 };
    snprintf(st, 127, "%d", (int32_t)x);
    size_t len = strlen(st);
    m_ptr = alloc_str(len);
    memcpy(m_ptr.get(), st, len);
    m_ptr[len] = 0;
    m_len = len;
}

str::str(int8_t x) {
    char st[128] = { 0 };
    snprintf(st, 127, "%d", (int32_t)x);
    size_t len = strlen(st);
    m_ptr = alloc_str(len);
    memcpy(m_ptr.get(), st, len);
    m_ptr[len] = 0;
    m_len = len;
}

str::str(double x) {
    char st[128] = { 0 };
    snprintf(st, 127, "%lf", (double)x);
    size_t len = strlen(st);
    m_ptr = alloc_str(len);
    memcpy(m_ptr.get(), st, len);
    m_ptr[len] = 0;
    m_len = len;
}

shared_ptr<char[]> alloc_str(size_t len) {


#if __cpp_lib_three_way_comparison >= 201907
    shared_ptr<char[]> out1 = make_shared<char[]>(len + 1);
#else
    shared_ptr<char[]> out1 = shared_ptr<char[]>(new char[len + 1]);
#endif
    return out1;
}

void TimerManager::update(float delta_time) {
    vector<Timer*> done;
    m_timers.for_each([&](Timer& timer) {
        timer.elapsed_time += delta_time;
        if (timer.elapsed_time >= timer.lifetime or !timer.only_call_at_end) {
            (timer.on_update)(timer);
        }
        if (timer.elapsed_time >= timer.lifetime) {
            done.push_back(&timer);
        }
        });
    for (Timer* i : done) {
        m_timers.destroy(i);
    }
}

void TimerManager::drop_timer(Timer* timer) {
    m_timers.destroy(timer);
}

Timer* TimerManager::new_timer(Timer time) {
    return m_timers.create(time);
}
Timer* TimerManager::timer(function<void(Timer&)> to_run, float time) {
    Timer timer;
    timer.on_update = to_run;
    timer.elapsed_time = 0.0;
    timer.only_call_at_end = true;
    timer.lifetime = time;
    return this->new_timer(timer);
}

Timer* TimerManager::repeating_timer(function<void(Timer&)> to_run, float time) {
    Timer timer;
    timer.on_update = to_run;
    timer.elapsed_time = 0.0;
    timer.only_call_at_end = false;
    timer.lifetime = time;
    return this->new_timer(timer);
}