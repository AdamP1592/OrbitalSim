#pragma once
#include <vector>
#include "components/Moveable.hpp"
#include "components/Position.hpp"
#include "components/CellIndex.hpp"
#include "FreeList.hpp"
#include "ParticleView.hpp"
struct ParticleStorage{
    // store everything in a freelist for locality and fixed indexing
    // and no internal compacting 
    FreeList<Position> positionComponents;
    FreeList<Moveable> moveComponents;
    FreeList<CellIndex> cellMetaComponents;

    void resizeStorage(int newSize);
    void extendStorage(int slotsNeeded);
    // returns index it's placed at
    int addParticle(vec3<double> pos, vec3<double> velocity, double mass, size_t cellIndex);
    // returns 0 if there is no maintenace requrest and 1 if there is
    bool removeParticle(size_t index);
    // makes a temporary particle view for a given index
    ParticleView getParticle(size_t index);
    // sets an existing particle entry to these new values. Unsafe, but efficient. 
    void setParticle(size_t index, vec3<double> pos, vec3<double> velocity, double mass, size_t cellIndex);
    size_t size();
};
