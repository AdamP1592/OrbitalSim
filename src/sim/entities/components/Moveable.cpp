#include "Moveable.hpp"
void MoveableFunctions::addKickLF(Moveable& m, const vec3<double>& a, double dt){
    m.velocity += a * (dt / 2);
}
void MoveableFunctions::addKickEuler(Moveable& m, const vec3<double>& a, double dt){
    m.v += a * dt;
}
