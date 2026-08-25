#pragma once
#include <cstdint>
#include <cmath>
#include <vector>
#include <memory>
#include <string>
using std::vector;
using std::string_view;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;

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
        return this->angle_to({ 1., 0. });
    }

};

class str {
    std::shared_ptr<char[]> m_ptr;
public:
    str();
    explicit str(const char* st);
    explicit str(string_view str);
    explicit str(const string& st);
    operator string_view();
    operator const char* () const;
    operator char* ();
    str operator+(string_view base) const;
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

