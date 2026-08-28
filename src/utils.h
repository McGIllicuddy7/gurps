#pragma once
#include <cstdint>
#include <cmath>
#include <vector>
#include <memory>
#include <string>
#include <array>
#include <cassert>
#include <functional>
#include <mutex>
using std::vector;
using std::string_view;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;
using std::unique_ptr;
using std::array;
using std::function;
extern "C" [[noreturn]] void raise(int32_t);

struct int2 {
    int32_t x;
    int32_t y;
    inline constexpr int2 operator+(const int2& rhs) const noexcept {
        return int2{ x + rhs.x, y + rhs.y };
    }
    inline constexpr int2 operator-(const int2& rhs) const noexcept {
        return int2{ x - rhs.x, y - rhs.y };
    }
    inline constexpr int2 operator*(const int rhs) const noexcept {
        return int2{ x * rhs, y * rhs };
    }
    inline constexpr int2 operator/(const int rhs) const noexcept {
        return int2{ x / rhs, y / rhs };
    };
    inline constexpr bool operator==(const int2& rhs)const noexcept {
        return rhs.x == x && rhs.y == y;
    }
    inline constexpr int dot(const int2& rhs) const noexcept {
        return x * rhs.x + y * rhs.y;
    }
    inline constexpr float len() const noexcept {
        return sqrt(x * x + y * y);
    }
};

struct float2 {
    float x;
    float y;
    inline constexpr float2 operator+(const float2& rhs) const noexcept {
        return float2{ x + rhs.x, y + rhs.y };
    }
    inline constexpr float2 operator-(const float2& rhs) const noexcept {
        return float2{ x - rhs.x, y - rhs.y };
    }
    inline constexpr float2 operator*(const float rhs) const noexcept {
        return float2{ x * rhs, y * rhs };
    }
    inline constexpr float2 operator/(const float rhs) const noexcept {
        return float2{ x / rhs, y / rhs };
    }
    inline constexpr bool operator==(const float2& rhs)const noexcept {
        return rhs.x == x && rhs.y == y;
    }
    inline constexpr float dot(const float2& rhs) const noexcept {
        return x * rhs.x + y * rhs.y;
    }
    inline constexpr float len() const noexcept {
        return sqrt(x * x + y * y);
    }
    inline constexpr float angle_to(const float2& rhs) const noexcept {
        return acos(this->dot(rhs) / (this->len() * rhs.len()));
    }
    inline constexpr float angle() const noexcept {
        return atan2(this->y, this->x);
    }
    inline constexpr float2 rotated_by(float theta) {
        float len = this->len();
        float angle = this->angle() + theta;
        float x = cos(angle) * len;
        float y = sin(angle) * len;
        return { x,y };
    }
};
struct float3 {
    float x, y, z;
    inline constexpr float3  operator+(const float3& rhs) const noexcept {
        return { x + rhs.x, y + rhs.y, z + rhs.z };
    }
    inline constexpr float3 operator-(const float3& rhs) const noexcept {
        return { x - rhs.x, y - rhs.y, z - rhs.z };
    }
    inline constexpr float3 operator*(float rhs) const noexcept {
        return { x * rhs, y * rhs, z * rhs };

    }
    inline constexpr float3 operator/(float rhs) const noexcept {
        return { x / rhs, y / rhs, z * rhs };
    }
    inline constexpr float dot(const float3& rhs) const noexcept {
        return rhs.x * x + rhs.y * y + rhs.z * z;
    }
    inline constexpr float len() const noexcept {
        return sqrt(x * x + y * y + z * z);
    }
    inline constexpr float angle(const float3& rhs)const noexcept {
        return acos(this->dot(rhs) / (this->len() * rhs.len()));
    }
};

class str {
    std::shared_ptr<char[]> m_ptr;
    size_t m_len = 0;
public:
    str();
    str(const char* st);
    explicit str(string_view str);
    explicit str(const string& st);
    explicit str(int32_t x);
    explicit str(uint32_t x);
    explicit str(int64_t x);
    explicit str(uint64_t x);
    explicit str(int16_t x);
    explicit str(uint16_t x);
    explicit str(int8_t x);
    explicit str(uint8_t x);
    explicit str(float x);
    explicit str(double x);
    explicit str(char c);
    operator string_view();
    operator const char* () const;
    operator char* ();
    str operator+(string_view base) const;
    void operator += (string_view base);
    const char& operator[](size_t idx) const;
    char& operator[](size_t idx);
    size_t size()const;
    char* begin();
    char* end();
    char* ptr();
    const char* const_ptr() const;
    const char* begin()const;
    const char* end()const;
    string_view sub_string(size_t start, size_t end)const;
    str clone() const;
};

template<typename T> class arr {
    T* m_ptr;
    size_t m_len;
public:
    arr() {
        m_len = 0;
        m_ptr = 0;
    }
    arr(T* ptr, size_t len) {
        m_ptr = ptr;
        m_len = len;
    }
    arr(vector<T>& v) {
        if (v.size()) {
            m_ptr = &v[0];
            m_len = v.size();
        }
        else {
            m_ptr = nullptr;
            m_len = 0;
        }
    }
    template<size_t COUNT> arr(array<T, COUNT>& ptr) {
        m_len = COUNT;
        m_ptr = &ptr[0];
    }

    size_t size() const {
        return m_len;
    }
    const T& operator [] (size_t idx)const {
        assert(idx < m_len);
        return m_ptr[idx];
    }
    T& operator [] (size_t idx) {
        assert(idx < m_len);
        return m_ptr[idx];
    }
    const T* begin() const {
        return m_ptr;
    }
    const T* end() const {
        return m_ptr + m_len;
    }
    T* begin() {
        return m_ptr;
    }
    T* end() {
        return m_ptr + m_len;
    }
};

template <typename T> class PtrSet {
    std::mutex m_lock;
    vector<unique_ptr<T>> m_values;
public:
    T* create(T value) {
        m_lock.lock();
        unique_ptr<T> v = make_unique<T>(value);
        T* out = v.get();
        for (size_t i = 0; i < m_values.size(); i++) {
            if (!m_values[i]) {
                m_values[i] = std::move(v);
                m_lock.unlock();
                return out;
            }
        }
        m_values.emplace_back(std::move(v));
        m_lock.unlock();
        return out;
    }

    T* create(T&& value) {
        m_lock.lock();
        unique_ptr<T> v = make_unique<T>(value);
        T* out = v.get();
        for (size_t i = 0; i < m_values.size(); i++) {
            if (!m_values[i]) {
                m_values[i] = std::move(v);
                m_lock.unlock();
                return out;
            }
        }
        m_values.emplace_back(std::move(v));
        m_lock.unlock();
        return out;
    }

    void destroy(T* t) {
        m_lock.lock();
        for (size_t i = 0; i < m_values.size(); i++) {
            if (m_values[i].get() == t) {
                m_values[i] = nullptr;
                m_lock.unlock();
                return;
            }
        }
        m_lock.unlock();
    }

    vector<T*> values() {
        m_lock.lock();
        vector<T*> list;
        for (auto& i : m_values) {
            if (i) {
                list.push_back(i.get());
            }
        }
        m_lock.unlock();
        return list;
    }

    vector<const T*> const_values() {
        m_lock.lock();
        vector<const T*> list;
        for (auto& i : m_values) {
            if (i) {
                list.push_back(i.get());
            }
        }
        m_lock.unlock();
        return list;
    }

    void for_each(std::function<void(T&)> to_run) {
        for (size_t i = 0; i < m_values.size(); i++) {
            if (m_values[i]) {
                to_run(*m_values[i]);
            }
        }
    }
    void clear() {
        m_values.clear();
    }
};

struct Timer {
    bool only_call_at_end;
    float lifetime;
    float elapsed_time;
    function<void(Timer&)> on_update;
};

class TimerManager {
    PtrSet<Timer> m_timers;
public:
    void update(float delta_time);
    void drop_timer(Timer* timer);
    Timer* new_timer(Timer time);
    Timer* timer(function<void(Timer&)> to_run, float time);
    Timer* repeating_timer(function<void(Timer&)> to_run, float time);
};



#define todo(...) printf("TODO file:%s line:%d " __VA_OPT__("%s") "\n", __FILE__, __LINE__ __VA_OPT__(,) __VA_ARGS__); raise(5);

