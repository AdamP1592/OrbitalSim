#ifndef VEC3_HPP
#define VEC3_HPP
#include <cmath>
#include <type_traits>
#include <concepts>
#include <iostream>


/*
NOTE: SCALAR AND STANDARD OPERATORS ARE SEPARATE FOR PERFORMANCE
*/
template <typename T>
struct vec3{
    T x, y, z;
    // CONSTRUCTORS

    // standard construction(either empty or filled)
    vec3(): x(), y(), z() {}
    vec3(T _x, T _y, T _z): x(_x), y(_y), z(_z){}
    
    // for explicit scalar construction
    vec3(T val): x(val), y(val), z(val) {}

    
    // for explicit casting of float to non float conversion, and implicit casting within the two domains.

    template <typename U>
    requires std::is_constructible_v<T, U>
    explicit(
        ((std::is_floating_point_v<U> && !std::is_floating_point_v<T>) ||
        (!std::is_floating_point_v<U> && std::is_floating_point_v<T>))
    )
    vec3(const vec3<U>& other)
        : x(other.x), y(other.y), z(other.z)
    {}
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


    bool allLessThan(const T val) const {
        return x < val && y < val && z < val;
    }
    bool allGreaterThan(const T val) const {
        return x > val && y > val && z > val;
    }


    
};
namespace math{
    
    // STATELESS FUNCTIONS


    // NUMERICAL HELPERS: 

    template<typename T>
    vec3<T> max(vec3<T>& a, vec3<T>& b){
        vec3<T> m;

        m.x = std::max(a.x, b.x);
        m.y = std::max(a.y, b.y);
        m.z = std::max(a.z, b.z);
        
        return m;
    }   
    template<typename T>
    vec3<T> min(vec3<T>& a, vec3<T>& b){
        vec3<T> m;

        m.x = std::min(a.x, b.x);
        m.y = std::min(a.y, b.y);
        m.z = std::min(a.z, b.z);
        
        return m;
    }

    // TYPE SPECIFIC FUNCTION SPECIFIERS
    template<typename Float_T>
    concept Floating = std::is_floating_point_v<Float_T>;

    template<typename Signed_Numeric_T>
    concept Signed_Numeric = std::is_arithmetic_v<Signed_Numeric_T> && std::is_signed_v<Signed_Numeric_T>;

    //ROUNDING
    template<Floating T>
    vec3<T> floor(const vec3<T>& v) {
        return {
            static_cast<T>(std::floor(v.x)),
            static_cast<T>(std::floor(v.y)),
            static_cast<T>(std::floor(v.z))
        };
    }
    template<Floating T>
    vec3<T> ceil(const vec3<T>& v) {
        return {
            static_cast<T>(std::ceil(v.x)),
            static_cast<T>(std::ceil(v.y)),
            static_cast<T>(std::ceil(v.z))
        };
    }
    template<Floating T>
    vec3<T> round(const vec3<T>& v) {
        return {
            static_cast<T>(std::round(v.x)),
            static_cast<T>(std::round(v.y)),
            static_cast<T>(std::round(v.z))
        };
    }
    template<Signed_Numeric T>
    vec3<T> abs(const vec3<T>& v){
        return {
            static_cast<T>(std::abs(v.x)),
            static_cast<T>(std::abs(v.y)),
            static_cast<T>(std::abs(v.z))
        };
    }

    template<Signed_Numeric T>
    vec3<T> trunc(const vec3<T>& v){
        return {
            static_cast<T>(std::trunc(v.x)),
            static_cast<T>(std::trunc(v.y)),
            static_cast<T>(std::trunc(v.z))
        };
    }

    // STABILITY HELPERS
    template<Floating T>
    bool roughEquals(vec3<T> a, vec3<T> b, T eps = 1e-7){
        return math::abs(a - b).allLessThan(eps);
    }

    template<Floating T>
    bool is_zero(vec3<T> v, T eps = 1e-7){
        vec3<T> zeroV;

        return roughEquals(v, zeroV, eps);
    }

    // LOGGING
    template<typename T>
    std::ostream& operator<<(std::ostream& os, const vec3<T>& v){
        return os << "{" << v.x << ", " << v.y << ", " << v.z;
    }
    

}


#endif