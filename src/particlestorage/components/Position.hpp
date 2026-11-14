#ifndef POSITION_HPP
#define POSITION_HPP
#include "vec3.hpp"
struct Position{
    vec3<double> pos;
    Position(vec3<double> pos_);
    void set(vec3<double> newPos);
};
#endif