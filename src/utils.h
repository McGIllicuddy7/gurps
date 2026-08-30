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
#include <raylib.h>
#include <raymath.h>
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

struct g_int2 {
    int32_t x;
    int32_t y;
    inline constexpr g_int2 operator+(const g_int2& rhs) const noexcept {
        return g_int2{ x + rhs.x, y + rhs.y };
    }
    inline constexpr g_int2 operator-(const g_int2& rhs) const noexcept {
        return g_int2{ x - rhs.x, y - rhs.y };
    }
    inline constexpr g_int2 operator*(const int rhs) const noexcept {
        return g_int2{ x * rhs, y * rhs };
    }
    inline constexpr g_int2 operator/(const int rhs) const noexcept {
        return g_int2{ x / rhs, y / rhs };
    };
    inline constexpr bool operator==(const g_int2& rhs)const noexcept {
        return rhs.x == x && rhs.y == y;
    }
    inline constexpr int dot(const g_int2& rhs) const noexcept {
        return x * rhs.x + y * rhs.y;
    }
    inline constexpr float len() const noexcept {
        return sqrt(x * x + y * y);
    }
};

using g_float2 = Vector2;
using g_float3 = Vector3;

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

template <typename T> class GPtrSet {
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

struct GTimer {
    bool only_call_at_end;
    float lifetime;
    float elapsed_time;
    function<void(GTimer&)> on_update;
};

class GTimerManager {
    GPtrSet<GTimer> m_timers;
public:
    void update(float delta_time);
    void drop_timer(GTimer* timer);
    GTimer* new_timer(GTimer time);
    GTimer* timer(function<void(GTimer&)> to_run, float time);
    GTimer* repeating_timer(function<void(GTimer&)> to_run, float time);
};



#define todo(...) printf("TODO file:%s line:%d " __VA_OPT__("%s") "\n", __FILE__, __LINE__ __VA_OPT__(,) __VA_ARGS__); raise(5);

