#include <thread>
#include <execution>
#include <algorithm>
#include <vector>
#include "sim/math/vec3.hpp"
#include "sim/physics/gravity/MassMesh.hpp"

class MassMesh{

    const int numNodesPerDimension = 512;
    vec3<double> d;

    public:
        int numLogicCores;
        // store x, y, z, mass of each node
        std::vector<float> mesh;
        
        MassMesh(vec3<double> dims= vec3<double>(100)){
            numLogicCores = std::thread::hardware_concurrency();
            d = dims / numNodesPerDimension;
        }
        void init_cell_policy(){
            
        }

        // MASS OPERATIONS
        void clearMesh(){
            //zerofill the bytes for the mesh in parallel
            std::fill(std::execution::par, mesh.begin(), mesh.end(), 0.0f);
        }
        void addMasses(std::vector<vec3<double>> pos, std::vector<double> mass){
            //todo
        }
        void addMass(vec3<double> pos, double mass){
            vec3<double> cell = floor(pos / d);

            // get the cell corner
            vec3<double> corner = cell * d; 
            
            //for distributing mass across each dimension via cic weighting
            vec3<double> dd = (pos - corner) / d;
            
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
                        int nodeIndex = getNodeIndex({cell.x + i, cell.y + j, cell.z + k});
                        mesh[nodeIndex] += mass * wx * wy * wz;
                    }
                }
            }
        }

        // Node Index/Position helper methods
        int getNodeIndex(vec3<double> pos){
            vec3<int> cell = pos/d;
            
            cell.x = std::min(cell.x, numNodesPerDimension - 1);
            cell.y = std::min(cell.y, numNodesPerDimension - 1);
            cell.z = std::min(cell.z, numNodesPerDimension - 1);

            return cell.x + (cell.y * numNodesPerDimension) + (cell.z * numNodesPerDimension * numNodesPerDimension);
        }
        vec3<int> getPos(int index){
            
            int k = index / (numNodesPerDimension * numNodesPerDimension);
            int j = (index / numNodesPerDimension) % numNodesPerDimension;
            int i = index % numNodesPerDimension;

            return {i, j, k};
        }

        // DEBUG

        

};