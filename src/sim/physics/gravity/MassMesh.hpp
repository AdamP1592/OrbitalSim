#ifndef MASS_MESH_H
#define MASS_MESH_H

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
        MassMesh(vec3<double> dims = vec3<double>(100),
                    vec3<double> realDimsMin_ = vec3<double>(0.0, 0.0, 0.0),
                    vec3<double> realDimsMax_ = vec3<double>(1.0, 1.0, 1.0));
        void clearMesh();
        void addMasses(std::vector<vec3<double>> pos, std::vector<double> mass);
        void addMass(vec3<double>pos, double mass);
        void setNode(int index, float mass);
};

#endif
