#include <thread>
#include <execution>
#include <algorithm>
#include <vector>
#include "vec3.hpp"
#include "physics/gravity/MassMesh.hpp"
#include "physics/gravity/MeshContext.hpp"


MassMesh::MassMesh(vec3<double> dims = vec3<double>(100),
                    vec3<double> realDimsMin_ = {0, 0, 0},
                    vec3<double> realDimsMax_ = {1, 1, 1}){
    numLogicCores = std::thread::hardware_concurrency();
    d = dims / numNodesPerDimension;
    realMin = realDimsMin_;
    realMax = realDimsMax_;

    ctx.meshDims = d;
    ctx.realDimsMax = realDimsMax_;
    ctx.realDimsMin = realDimsMin_;
    
}
void MassMesh::init_cell_policy(){
    //
}

        // MASS OPERATIONS
void MassMesh::clearMesh(){
    //zerofill the bytes for the mesh in parallel
    std::fill(std::execution::par, mesh.begin(), mesh.end(), 0);
}
void MassMesh::addMasses(std::vector<vec3<double>> pos, std::vector<double> mass){
    //todo multithreaded, 9 passes so there are no shared nodes
}
void MassMesh::addMass(vec3<double> pos, double mass){
    vec3<double> cell = math::floor(pos / d);

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

void setNode(int index, float mass){
    mesh[index] = mass;
}

// Node Index/Position helper methods
int MassMesh::getNodeIndexFromPos(vec3<double> pos){
    vec3<int> cell = vec3<int>(pos/d);
    
    cell.x = std::min(cell.x, numNodesPerDimension - 1);
    cell.y = std::min(cell.y, numNodesPerDimension - 1);
    cell.z = std::min(cell.z, numNodesPerDimension - 1);

    return cell.x + (cell.y * numNodesPerDimension) + (cell.z * numNodesPerDimension * numNodesPerDimension);
}
vec3<double> MassMesh::getPos(int index){
    
    double k = index / (numNodesPerDimension * numNodesPerDimension);
    double j = (index / numNodesPerDimension) % numNodesPerDimension;
    double i = index % numNodesPerDimension;

    return {i, j, k} * d;
}
int MassMesh::getNodeIndexFromNodeCoord(vec3<int> nodeCoords){
    nodeCoords.x = std::clamp(nodeCoords.x, 0, numNodesPerDimension - 1);
    nodeCoords.y = std::clamp(nodeCoords.y, 0, numNodesPerDimension - 1);
    nodeCoords.z = std::clamp(nodeCoords.z, 0, numNodesPerDimension - 1);

    return nodeCoords.x
        + nodeCoords.y * numNodesPerDimension
        + nodeCoords.z * numNodesPerDimension * numNodesPerDimension;
}