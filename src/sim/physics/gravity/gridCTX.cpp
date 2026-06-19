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
        vec3<int> cell = vec3<int>(pos/d);
        
        cell.x = std::min(cell.x, numNodesPerDim - 1);
        cell.y = std::min(cell.y, numNodesPerDim - 1);
        cell.z = std::min(cell.z, numNodesPerDim - 1);

        return cell.x + (cell.y * numNodesPerDim) + (cell.z * numNodesPerDim * numNodesPerDim);
    }
    int getNodeIndexFromNodeCoord(vec3<int> nodeCoords){
        nodeCoords.x = std::clamp(nodeCoords.x, 0, numNodesPerDim - 1);
        nodeCoords.y = std::clamp(nodeCoords.y, 0, numNodesPerDim - 1);
        nodeCoords.z = std::clamp(nodeCoords.z, 0, numNodesPerDim - 1);

        return nodeCoords.x
            + nodeCoords.y * numNodesPerDim
            + nodeCoords.z * numNodesPerDim * numNodesPerDim;
    }
    vec3<double> getPos(int index){
    
        double k = index / (numNodesPerDim * numNodesPerDim);
        double j = (index / numNodesPerDim) % numNodesPerDim;
        double i = index % numNodesPerDim;

        return {i, j, k} * d;
    }

};