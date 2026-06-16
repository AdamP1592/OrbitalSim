#ifndef POSITION_HPP
#define POSITION_HPP
#include "vec3.hpp"
struct Position{
    vec3<double> pos;
}
namespace PositionFunctions{
    void drift(vec3<double>& v, vec3<double> p, double dt);
}

#endif