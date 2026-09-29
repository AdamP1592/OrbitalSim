#include "ParticleStorage.hpp"
#include "ParticleView.hpp"
#include "vec3.hpp"
int ParticleStorage::addParticle(vec3<double> pos, vec3<double> velocity, double mass, size_t cellIndex){
    positionComponents.push(Position(pos));
    moveComponents.push(Moveable(mass, velocity));
    return cellMetaComponents.push(CellIndex(cellIndex));
}
/**
 * Removes a particle and returns a bool flag for a maintenance call.
 * 
 * @param size_t index the index to remove from each of the component vectors
 * @important Currently only returns false, but will eventually make a maintenance call when the amount of freed entries surpasses a certain threshold. 
 */
bool ParticleStorage::removeParticle(size_t index){
    positionComponents.remove(index);
    moveComponents.remove(index);
    cellMetaComponents.remove(index);
    return false;
}
ParticleView ParticleStorage::getParticle(size_t index){
    
    Position& p = positionComponents.get(index);
    Moveable& mv = moveComponents.get(index);
    CellIndex& c = cellMetaComponents.get(index);

    ParticleView pv(p.pos, mv.velocity, mv.mass, c.cell);
    return pv;
}
// leaving the hanging out of bounds error to prevent repeated checks for batch update
void ParticleStorage::setParticle(size_t index, vec3<double> pos, vec3<double> velocity, double mass, size_t cellIndex){
    positionComponents.set(Position(pos), index);
    moveComponents.set(Moveable(mass, velocity), index);
    cellMetaComponents.set(CellIndex(cellIndex), index);
    
}
size_t ParticleStorage::size(){
    // just a relay function 
    return positionComponents.size() - positionComponents.freedSize();
}