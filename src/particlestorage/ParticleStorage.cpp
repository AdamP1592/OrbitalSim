#include "ParticleStorage.hpp"
#include "ParticleView.cpp"
#include "vec3.hpp"
int ParticleStorage::addParticle(vec3<double> pos, vec3<double> velocity, double mass, size_t cellIndex){
    positionComponents.push(Position(pos));
    moveComponents.push(Moveable(mass, velocity));
    return cellMetaComponents.push(cellIndex);
}
int ParticleStorage::removeParticle(size_t index){
    particleComponents.remove(index);
    moveComponents.remove(index);
    cellMetaComponents.remove(index);
}
ParticleView ParticleStorage::getParticle(size_t index){
    ParticleView pv;

    pv.pos = particleComponents.get(index);
    pv.mass = moveComponents.get(index);
    pv.cellIndex = cellMetaComponents.get(index);

    return pv;
}