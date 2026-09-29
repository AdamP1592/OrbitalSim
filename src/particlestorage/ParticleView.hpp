#pragma once
#include "vec3.hpp"
struct ParticleView{
    vec3<double> &position;
    vec3<double> &velocity;
    double &mass;
    size_t &cellIndex;
    ParticleView(vec3<double>& _position, vec3<double>& _velocity, double& _mass, size_t& _cellIndex) : 
    position(_position), velocity(_velocity), mass(_mass), cellIndex(_cellIndex) {}
};
