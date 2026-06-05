#ifndef MOVEABLE_HPP
#define MOVEABLE_HPP
#include "vec3.hpp"
struct Moveable{
    vec3<double> velocity;
    float mass;
}

namespace MoveableFunctions{
    void addKickLF(Moveable& m, const vec3<double>& a, double dt);
    void addKickEuler(Moveable& m, const vec3<double>& a, double dt)
}
#endif