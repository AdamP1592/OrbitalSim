#pragma once
#include <vector>
#include "components/Moveable.hpp"
#include "components/Position.hpp"
#include "components/CellIndex.hpp"
#include "FreeList.hpp"
#include "ParticleView.cpp"
struct ParticleStorage{
    // store everything in a freelist for locality and fixed indexing
    // and no internal compacting 
    FreeList<Position> positionComponents;
    FreeList<Moveable> moveComponents;
    FreeList<CellIndex> cellMetaComponents;

    // returns index it's placed at
    int addParticle(vec3<double> pos, vec3<double> velocity, double mass, int cellIndex);
    // returns 0 if there is no maintenace requrest and 1 if there is
    int removeParticle(size_t index);
    // makes a temporary particle view for a given index
    ParticleView getParticle(size_t index);
};
