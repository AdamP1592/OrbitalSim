#include <thread>
#include <execution>
#include <algorithm>
#include <cmath>
#include <vector>
#include "vec3.hpp"
#include "physics/gravity/GridCTX.cpp"
#include "physics/gravity/MassMesh.hpp"



MassMesh::MassMesh(vec3<double> dims, vec3<double> realDimsMin_, vec3<double> realDimsMax_){
    
    //set the main params
    d = dims / numNodesPerDimension;
    realMin = realDimsMin_;
    realMax = realDimsMax_;

    int numNodesTotal = std::pow(numNodesPerDimension, 3);
    int sizeInFloats =  2 * (std::floor(numNodesTotal / 2) + 1);
    // pad the grid with 2 * (n/2 + 1) -> n + 2;
    mesh = std::vector<float>(numNodesTotal + 2);
    ctx = GridCTX(vec3<double>(realMin), vec3<double>(realMax), 100, 512);

}

// MASS OPERATIONS
void MassMesh::clearMesh(){
    //zerofill the bytes for the mesh in parallel
    std::fill(std::execution::par, mesh.begin(), mesh.end(), 0.0f);
}
void MassMesh::addMasses(std::vector<vec3<double>> pos, std::vector<double> mass){
    //todo multithreaded, 9 passes so there are no shared nodes
}
void MassMesh::addMass(vec3<double> pos, double mass){
    vec3<double> cell = vecmath::floor(pos / d);

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
                vec3<int> cellPos = ctx.gridToCell({cell.x + i, cell.y + j, cell.z + k});
                int nodeIndex = ctx.cellToIndex(cellPos);
                mesh[nodeIndex] += mass * wx * wy * wz;
            }
        }
    }
}

void MassMesh::setNode(int index, float mass){
    mesh[index] = mass;
}
