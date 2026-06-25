#ifndef MASS_MESH_H
#define MASS_MESH_H
#include "vec3.hpp"
#include "physics/gravity/MeshContext.hpp"
#include <vector>
class MassMesh{
    vec3<double> d;
    vec3<double> realMin;
    vec3<double> realMax;
   
    const int numNodesPerDimension = 512;
    public:
        GridCTX ctx;

        // ALL OPERATIONS MUST CONVERT FRON INT64 TO FLOAT BEFORE USE
        std::vector<float> mesh;
        int numLogicCores;
        MassMesh(vec3<double> dims);
        MeshContext ctx;
        void clearMesh();
        void addMasses(std::vector<vec3<double>> pos, std::vector<double> mass);
        void addMass(vec3<double>pos, double mass);
}

#endif
