#pragma once
#include "physics/gravity/GridCTX.cpp"
struct SimCTX{
    GridCTX grid;
    double dt;
    int64_t scale;

};