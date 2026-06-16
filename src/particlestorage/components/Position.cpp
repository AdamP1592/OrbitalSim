#include "Position.hpp"
void PositionFunctions::drift(vec3<double> p, vec3<double>& v, double dt){
    p += v * dt;
}