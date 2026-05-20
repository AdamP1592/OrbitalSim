#ifndef VEC3_HPP
#define VEC3_HPP
#include <cmath>
#include <type_traits>
template <typename T>

/*
NOTE: SCALAR AND STANDARD OPERATORS ARE SEPARATE FOR PERFORMANCE
*/
struct vec3{
    T x, y, z;
    // CONSTRUCTORS

    // standard construction(either empty or filled)
    vec3(): x(), y(), z() {}
    vec3(T _x, T _y, T _z): x(_x), y(_y), z(_z){}
    
    // for explicit scalar construction
    explicit vec3(T val): x(val), y(val), z(val) {}

    // for casting
    template<typename U>
    vec3(const vec3<U>& other):
        x(other.x), y(other.y), z(other.z) {}

    // ARITHMETIC OPERATIONS

    // ADDITION
    vec3<T> operator+(const vec3<T>& other) const{
        return vec3<T>(x + other.x, y + other.y, z + other.z);
    }
    vec3<T> operator+(const T addend) const {
        return vec3<T>(x + addend, y + addend, z + addend);
    }
    
    vec3<T>& operator+=(const vec3<T>& other){
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    vec3<T>& operator+=(const T addend){
        x += addend;
        y += addend;
        z += addend;
        return *this;
    }

    // SUBTRACTION
    vec3<T> operator-(const vec3<T>& other) const{
        return vec3<T>(x - other.x, y - other.y, z - other.z);
    }
    vec3<T> operator-(const T sub) const{
        return vec3<T>(x - sub, y - sub, z - sub);
    }

    vec3<T>& operator-=(const vec3<T>& other){
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }vec3<T>& operator-=(const T sub){
        x -= sub;
        y -= sub;
        z -= sub;
        return *this;
    }
    
    // DIVISION
    vec3<T> operator/(const vec3<T>& other) const{
        return vec3<T>(x / other.x, y / other.y, z / other.z);
    }
    vec3<T> operator/(const T div) const{
        return vec3<T>(x / div, y / div, z / div);
    }

    vec3<T>& operator/=(const vec3<T>& other){
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }
    vec3<T>& operator/=(const T div){
        x /= div;
        y /= div;
        z /= div;
        return *this;
    }

    // MULTIPLICATION
    vec3<T> operator*(const vec3<T>& other) const{
        return vec3<T>(x * other.x, y * other.y, z * other.z);
    }
    vec3<T> operator*(const T fact) const{
        return vec3<T>(x * fact, y * fact, z * fact);
    }

    vec3<T>& operator*=(const vec3<T>& other){
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }
    vec3<T>& operator*=(const T fact){
        x *= fact;
        y *= fact;
        z *= fact;
        return *this;
    }
    
    // LOGIC OPERATORS 

    // default member comparison methods inlcuded
    auto operator<=>(const vec3&) const = default;
    
};
// FREE FUNCTIONS

// ROUNDING

template<typename T>
requires std::is_floating_point_v<T>
vec3<T> floor(const vec3<T>& v) {
    return {
        static_cast<T>(std::floor(v.x)),
        static_cast<T>(std::floor(v.y)),
        static_cast<T>(std::floor(v.z))
    };
}


#endif