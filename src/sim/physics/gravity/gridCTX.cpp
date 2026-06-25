#include "vec3.hpp"
struct GridCTX{
    vec3<double>& minRealCoords;
    vec3<double>& maxRealCoords;
    vec3<double>& gridMax;
    int numNodesPerDim;
    GridCTX(vec3<double>& minRealCoords_, vec3<double>& maxRealCoords_, vec3<double>& gridMax_, int numNodesPerDim_){
        minRealCoords = minRealCoords_;
        maxRealCoords = maxRealCoords_;
        gridMax = gridMax_;
        numNodesPerDim = numNodesPerDim_;
    }
    vec3<double> realToGrid(vec3<double> point){
        //grid coords are never negative so it's min on grid is 0
        return ((point - minRealCoords) / (maxRealCoords - minRealCoords)) * (gridMax);
    }
    vec3<double> gridToReal(vec3<double> gridPoint){
        return minRealCoords + ((gridPoint / gridMax) * (maxRealCoords - minRealCoords));
    }
    /**
     * Converts real coords to the cell index of the closest node
     * @param pos position in real-coords
     * @returns index of the cell as it's stored.
     */
    int realToIndex(vec3<double> pos){
        pos = realToGrid(pos);
        vec3<int> cell = vec3<int>(pos/d);
        
        return gridToIndex(cell);
    }

    /**
     * @param pos position in grid coords
     */
    int gridToIndex(vec3<double>& pos){
        vec3<int> cell = vec3<int>(pos/d);
        return gridToIndex(cell);
    }
    /**
     * @param nodeCoords position as an unflattened index;
     */
    int gridToIndex(vec3<int> nodeCoords){
        int zStride = numNodesPerDim + 2;

        nodeCoords.x = std::clamp(nodeCoords.x, 0, numNodesPerDim - 1);
        nodeCoords.y = std::clamp(nodeCoords.y, 0, numNodesPerDim - 1);
        nodeCoords.z = std::clamp(nodeCoords.z, 0, numNodesPerDim - 1);

        return nodeCoords.x * numNodesPerDim * zStride
            + nodeCoords.y * zStride
            + nodeCoords.z;
    }
    vec3<int> indexToGrid(int index){
        int zStride = numNodesPerDim + 2;
        int yStride = numNodesPerDim * zStride;

        int x = index / yStride;
        int y = (index % yStride) / zStride;
        int z = index % zStride;

        return {x, y, z};
    }
    vec3<double> getCellPos(vec3<double> particlePos){
        realToGrid(particlePos);
        vec3<double> cell = math::floor(pos / d);

    }
    vec3<double> getDD(vec3<double> cellPos){
        // get the cell corner
        vec3<double> corner = cell * d; 
        
        //for distributing mass across each dimension via cic weighting
        return (pos - corner) / d;
    
    }

};