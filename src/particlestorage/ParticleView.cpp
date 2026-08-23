#include "vec3.hpp"
struct ParticleView{
    vec3<double> &position;
    double &mass;
    size_t &cellIndex;
};