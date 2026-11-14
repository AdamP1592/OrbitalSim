#ifndef MOVEABLE_HPP
#define MOVEABLE_HPP
#include "vec3.hpp"
struct Moveable{
    vec3<double> velocity;
    double mass;
    Moveable(double mass_, vec3<double> initialV);
};

namespace MoveableFunctions{
    void addKickLF(vec3<double>& v, const vec3<double>& a, const double dt);
    void addKickEuler(vec3<double>& v, const vec3<double>& a, const double dt);
    void drift(const vec3<double>& v, vec3<double>& position, const double dt);
}
#endif