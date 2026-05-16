#include <vector>
#include <tuple>
#include <thread>
#include <algorithm>

using namespace std;
class Field{

    const int numNodesPerDimension = 512;
    double dx, dy, dz;

    public:
        int numLogicCores;
        // store x, y, z, mass of each node
        vector<float> mesh;
        
        Field(double width = 100, double length = 100, double height = 100){
            numLogicCores = std::thread::hardware_concurrency();
            dx = width / numNodesPerDimension;
            dy = length / numNodesPerDimension;
            dz = height / numNodesPerDimension;
        }
        void clearMass(){
            //zerofill the bytes for the mesh, requires IEEE 754
            fill(mesh.begin(), mesh.end(), 0.0f);
        }
        void addMass(double x, double y, double z, double mass){

            int cell_i = (int)(x / dx);
            int cell_j = (int)(y / dy);
            int cell_k = (int)(z / dz);

            // get the cell corner (left/bottom/front node)
            double x0 = cell_i * dx;
            double y0 = cell_j * dy;
            double z0 = cell_k * dz;

            //for distributing mass across each node via cic weighting
            double ddx = (x - x0) / dx;
            double ddy = (y - y0) / dy;
            double ddz = (z - z0) / dz;
            
            // add mass to each node of the cell
            for(int i = 0; i <= 1; i++){
                for(int j = 0; j <= 1; j++){
                    for(int k = 0; k <= 1; k++){
                        // component weights for the corner 
                        double wx = (i == 0) ? 1.0 - ddx : ddx;
                        double wy = (j == 0) ? 1.0 - ddy : ddy;
                        double wz = (k == 0) ? 1.0 - ddz : ddz;

                        // add mass to node based on it's weight
                        int nodeIndex = getNodeIndex(cell_i + i, cell_j + j, cell_k + k);
                        mesh[nodeIndex] += mass *wx * wy * wz;
                    }
                }
            }
        }

        // Node Index/Position helper methods
        int getNodeIndex(double x, double y, double z){
            int i = x / dx;
            int j = y / dy;
            int k = z / dz;
            
            i = min(i, numNodesPerDimension - 1);
            j = min(j, numNodesPerDimension - 1);
            k = min(k, numNodesPerDimension - 1);

            return i + (j * numNodesPerDimension) + (k * numNodesPerDimension * numNodesPerDimension);
        }
        vector<int> getPos(int index){
            
            int k = index / (numNodesPerDimension * numNodesPerDimension);
            int j = (index / numNodesPerDimension) % numNodesPerDimension;
            int i = index % numNodesPerDimension;

            return {i, j, k};
        }

};