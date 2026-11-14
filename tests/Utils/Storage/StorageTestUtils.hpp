#pragma once

#include "ParticleView.hpp"
#include "ParticleStorage.hpp"
#include "SimCTX.hpp"

#include <cmath>
#include <cstdint>

ParticleStorage createParticleStorage(){
    ParticleStorage ps;
    
    ps.extendStorage(10);

    return ps;
}
SimCTX makeCtx(){
    SimCTX ctx;
    ctx.dt = 0.01;
    ctx.scale = double (1e10);
    ctx.grid = GridCTX(vec3<double>{0.0, 0.0, 0.0}, vec3<double>{11.0, 11.0, 11.0},
                       vec3<double>{16.0, 16.0, 16.0}, 16);
    return ctx;
}