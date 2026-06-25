
//basic template for all tasks, a constructor and a run function are the bare minimum and
struct Task{
    Task(){}
    void run(){}
    void getCompleted(){}
};

#include <vector>
#include <unordered_map>
#include "ParticleView.hpp"
#include "GridContext.hpp"
#include "vec3.hpp"
#include <atomic>
// A task for depositing mass onto the worker-local mesh atomically. 
struct DepositTask : Task{
    /**
     * DepositTask is a task that deposits particles onto a series of nodes.
     * @param particleViews_ a vector of the particles associated with the nodes given
     * @param globalNodeIndices a vector of the indices that the nodes are at on the global mesh
     * @param nodeReferences a vector of references to the worker-local shared mesh
     * @param ctx a MeshContext object that handles all position conversions.
     */
    std::vector<size_t> globalNodeIndices;
    DepositTask(
        std::vector<ParticleView&>& particleViews_,
        std::vector<size_t> globalNodeIndices_,
        std::vector<int64_t&> nodesReferences,
        MeshContext& ctx)   
    {   
        
        ctx = ctx_;
        particleVies = particleViews_;

        globalNodeIndices = globalNodeIndices_;
        constructNodeMap();
    }
    /**
     * Deposits 
     */
    void run(){
        for(ParticleView& p: particleViews_){
            vec3<double> cell = ctx.getCell(p.position);
            vec3<double> corner = ctx.getDD(cellPos);
            vec3<double> dd = pos - corner / d;

               // add mass to the 8 nearest nodes
            for(int i = 0; i <= 1; i++){
                for(int j = 0; j <= 1; j++){
                    for(int k = 0; k <= 1; k++){
                        // component weights for the corner
                        double wx = (i == 0) ? 1.0 - dd.x : dd.x;
                        double wy = (j == 0) ? 1.0 - dd.y : dd.y;
                        double wz = (k == 0) ? 1.0 - dd.z : dd.z;

                        // add mass to node based on it's weight
                        vec3<double> tmp = {cell.x + i, cell.y + j, cell.z + k};
                        int nodeIndex = ctx.gridToIndex({cell.x + i, cell.y + j, cell.z + k});
                        mesh[nodeIndex] += mass * wx * wy * wz;
                    }
                }
            }
        }
    }
    private:
        
        std::unordered_map<size_t, atomic<int64_t&>> nodes,
        
        std::vector<ParticleView>& particleViews;
        MeshContex& ctx;

        /**
         * Because tasks may be stolen, nodeMaps will have to be reconstructed after stealing, so this process is owned by the task
         */
        void constructNodeMap(){
            for(int i = 0; i < 8; i++){
                nodes[globalNodeIndices[i]] = nodeReferences[i];
            }
        }

}


