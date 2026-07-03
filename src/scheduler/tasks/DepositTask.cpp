// deposits all the masses onto the 8 grid points in the cell
#include <stdexcept>
#include <vector>
#include <array>
#include "tasks/Task.cpp"
#include "vec3.hpp"
#include "SimCTX.hpp"
struct DepositTask : Task{
    std::array<int64_t, 8> totalMass{};
    // an array that stores local node index-> global node index. Used for reduction.
    // values are implicitly mapped based on order of operations.
    std::array<int, 8> globalNodeIndices{};

    std::vector<ParticleView> particleViews;
    SimCTX simCTX;
    /**
     * Deposit task is a task that deposits all particles for a given cell onto a local-only mesh
     * 
     * @invariant task must contain only particles belonging to the same cell
     * @invariant this task must contain at least one particle
     * 
     * @param particleViews a vector of particle veiws to get utilized by the task
     * @param SimCTX a reference to the worker-local SimCTX object
     * 
     */
    DepositTask(std::vector<ParticleView> particleViews_, SimCTX& simCTX_)
        : particleViews(particleViews_), simCTX(simCTX_)
    {
        mapCoords();
    }
    /**
     * Deposits all particles in this task onto the local grid.
     */
    void run(){
        // initialize all values outside the loop to ensure cache-friendly performance without
        // relying on the compiler to handle it 
        
        // this will always be the same since particles in this exclusively belong to this cell
        vec3<int> base = simCTX.gridCTX.realToCell(particleViews[0].position);
        vec3<double> frac;

        double fx, fy, fz;
        double ifx, ify, ifz;
        
        double wx[2], wy[2], wz[2];

        for(ParticleView& p : particleViews){
            frac = simCTX.gridCTX.getCellFraction(p.position);

            fx = frac.x;
            fy = frac.y;
            fz = frac.z;
            
            ifx = 1.0 - fx;
            ify = 1.0 - fy; 
            ifz = 1.0 - fz;

            // set cic weights 
            wx[0] = ifx; wx[1] = fx;
            wy[0] = ify; wy[1] = fy;
            wz[0] = ifz; wz[1] = fz;
            
            //bit extraction for hot path loop to make it a bit cheaper.

            for(int i = 0; i < 8; i++){
                int dx = (i >> 2) & 1;
                int dy = (i >> 1) & 1;
                int dz = i & 1;
                
                totalMass[i] += static_cast<int64_t>(wx[dx] * wy[dy] * wz[dz] * p.mass * simCTX.scale);

            }
        }
    }
    /**
     * Orchestrates the mapping of coordinates from local index to global index
     * @invariant requires some particle to be within the task
     * @invariant requires all particles within the task to be stored within the same cell
     */
    void mapCoords(){
        if(particleViews.size() == 0){
            throw std::logic_error("Invariant Violated: DepositTask::mapCoords called on empty task");
        }
        mapCoords_();
    }
    
    private:
        /**
         * Handles mapping coordinates based on order of operation of the actual deposit
         */
        void mapCoords_(){
            vec3<int> base = simCTX.gridCTX.realToCell(particleViews[0].position);
            
            for(int i = 0; i < 8; i++){
                // setup for vector math
                vec3<int> d = {(i >> 2) & 1, (i >> 1) & 1,  i & 1};
                
                int globalIndex = simCTX.gridCTX.cellToIndex(base + d);
                globalNodeIndices[i] = globalIndex;
            }
        }
};

