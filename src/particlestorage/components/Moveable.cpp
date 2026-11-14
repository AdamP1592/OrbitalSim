#include "Moveable.hpp"
void MoveableFunctions::addKickLF(vec3<double>& v, const vec3<double>& a, const double dt){
    v += a * dt * 0.5;
}
void MoveableFunctions::addKickEuler(vec3<double>& v, const vec3<double>& a, const double dt){
    v += a * dt;
}
void MoveableFunctions::drift(const vec3<double>& v, vec3<double>& p, const double dt){
    p += v * dt;
}
Moveable::Moveable(double mass_, vec3<double> initialV = {0.0, 0.0, 0.0}) 
: mass(mass_), velocity(initialV) {};