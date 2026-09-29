#include <vector>
#include <string>
#include "ParticleStorage.hpp"
#include "ParticleView.hpp"
#include "LogHelper.hpp"


const vec3<double> pos = {0.0, 0.1, 0.2};
const vec3<double> vel = {0.3, 0.4, 0.5};

const double mass = 10.00;
const int cellInd = 0;

ParticleStorage makeOneParticle(){
    ParticleStorage ps;
    ps.addParticle(pos, vel, mass, cellInd);
    return ps;
}

int testParticleView(ParticleView& pv, const vec3<double> position, const vec3<double> velocity, const double mass, const int cellInd, std::string rootTest = "particleViewTest"){
    int failed = 0;

    if(!vecmath::roughEquals(pv.velocity, velocity)){
        printLog(pv.velocity, velocity, (rootTest + "_pv_velocity").c_str());
        failed++;
    }

    if(!vecmath::roughEquals(pv.position, position)){
        printLog(pv.position, position, (rootTest + "_pv_position").c_str());
        failed++;
    }

    if( pv.mass != mass){
        printLog(pv.mass, mass, (rootTest + "_pv_mass").c_str());
        failed++;
    }

    if( pv.cellIndex != cellInd){
        printLog(pv.cellIndex, cellInd, (rootTest + "_pv_cellMeta").c_str());
        failed++;
    }

    return failed;
}

int testRemoval(){
    ParticleStorage ps = makeOneParticle();
    ps.addParticle(vec3(0.0), vec3(0.1), 1.5, 1);
    ps.addParticle(vel, pos, 12.1, 1);
    
    int failure = 0;
    size_t size = ps.size();
    ps.removeParticle(1);

    //confirm if the removal was valid
    if(ps.size() != size - 1){
        printLog(ps.size(), size - 1, "ParticleStorage_removaltest" );
        failure++;
    }

    return failure;

}
int testPVUpdate(){
    double newMass = 11.111;
    ParticleStorage ps = makeOneParticle();


    ParticleView pv = ps.getParticle(0);
    pv.mass = newMass;
    ParticleView pv1 = ps.getParticle(0);

    if(pv1.mass != newMass){
        printLog(pv1.mass, newMass, "pv_update");
        return 1;
    }

    return 0;
}
int testSetter(){
    int failed = 0;
    ParticleStorage ps = makeOneParticle();

    ps.setParticle(0, vel, pos, 1.0, 1);

    ParticleView pv = ps.getParticle(0);
    failed += testParticleView(pv, vel, pos, 1.0, 1, "setterTest");
    
    return failed;
}
int testPush(){

    int failed = 0;
    
    ParticleStorage ps;
    ps.addParticle(pos, vel, mass, cellInd);
    std::cout << ps.moveComponents.get(0).velocity << std::endl;

    int moveSize = ps.moveComponents.size();
    int positionSize = ps.positionComponents.size();
    int metaSize = ps.cellMetaComponents.size();

    //confirm there is an entry in all components
    if(moveSize != 1){
        printLog("moveComponents.size() = " + std::to_string(moveSize), "moveComponents.size() = 1", "pushTest_move");
        failed++;
    }
    if(positionSize != 1){
        printLog("positionComponents.size() = " + std::to_string(positionSize), "positionComponents.size() = 1", "pushTest_pos");
        failed++;
    }
    if(metaSize != 1){
        printLog("cellMetaComponents.size() = " + std::to_string(metaSize), "cellMetaComponents.size() = 1", "pushTest_cellmeta");
        failed++;
    }
    ParticleView pv = ps.getParticle(0);

    //confirm particle view matches the construction 
    failed += testParticleView(pv, pos, vel, mass, cellInd, "pushTest");

    return failed;
}

int main(){
    int failure = 0;
    std::vector<int(*)()> testVector = {
        testPush,
        testSetter,
        testRemoval,
    };

    for (auto test : testVector){
        failure += test();
    }

    return failure;
}