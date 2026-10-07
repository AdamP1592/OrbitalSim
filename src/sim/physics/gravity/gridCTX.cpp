#include "vec3.hpp"
struct GridCTX {
    vec3<double> minRealCoords;
    vec3<double> maxRealCoords;
    vec3<double> gridMax;
    vec3<double> gridMin;
    vec3<double> d;
    int pad;
    int marginCells;
    int numNodesPerDim;
    int zStride;
    /**
     * GridCTX is a lightweight object for position and index conversion
     */
    GridCTX(){}
    GridCTX(vec3<double> minRealCoords_, vec3<double> maxRealCoords_,
            vec3<double> gridMax_, int numNodesPerDim_)
        : gridMax(gridMax_), numNodesPerDim(numNodesPerDim_),
          zStride(numNodesPerDim_ + 2)
    {
        d = gridMax_ / static_cast<double>(numNodesPerDim);
        // shrink resolution of the grid to add padding for cic weighting at the boundary
        gridMin = d * static_cast<double>(pad);               
        gridMax = gridMax_ - d * static_cast<double>(pad);
        d = gridMax_ / static_cast<double>(numNodesPerDim_);  

    }
    void setMinMax(vec3<double> minReal, vec3<double> maxReal){
        minRealCoords = minReal;
        maxRealCoords = maxReal;
    }

    /**
     * converts real coordinates to a virtual grid-coordinate system with a fixed range
     * @param point a point in the dynamic particle space
     * @returns the position in grid coordinates
     */
    vec3<double> realToGrid(vec3<double> point) const {
        return gridMin + ((point - minRealCoords) / (maxRealCoords - minRealCoords)) * gridMax;
    }
    /**
     * Converts a grid point into particle space coordinates
     * @param gridPoint a point in the static grid-space
     * @returns the position in particle space
     */
    vec3<double> gridToReal(vec3<double> gridPoint) const {
        return minRealCoords + ((gridPoint - gridMin) / (gridMax - gridMin)) * (maxRealCoords - minRealCoords);
    }
    /**
     * Converts directly from real to it's virtual index that can point to the position in flattened space
     * useful for cic weighting when you want to get adjacent nodes of a particle
     * @param point a point in particle space
     * @returns a vec3 of ints that represent it's virtual index.
     */
    vec3<int> realToCell(vec3<double> point) const {
        return gridToCell(realToGrid(point));
    }
    /**
     * Converts from grid space to virtual index of the node
     * useful if you've already gotten a node's virtual index and want the cic weights 
     * @param gridPoint
     * @returns nodes virtual index
     */
    vec3<int> gridToCell(vec3<double> gridPoint) const {
        const vec3<int> cell(vecmath::floor(gridPoint / d));
        return vecmath::clamp(cell, 0, numNodesPerDim - 1);
    }
    /**
     * Converts from the virtual node coords to the index in the flattened array
     * @param nodeCoords
     * @returns the real index of the node
     */
    int cellToIndex(vec3<int> nodeCoords) const {
        nodeCoords = vecmath::clamp(nodeCoords, 0, numNodesPerDim - 1);
        return nodeCoords.x * numNodesPerDim * zStride + nodeCoords.y * zStride + nodeCoords.z;
    }
    /**
     * Converts from the flattened index to the virtual 3d node index. 
     * Useful for debugging
     * @param indexToCell
     * @returns virtual 
     */
    vec3<int> indexToCell(int index) const {
        int yStride = numNodesPerDim * zStride;
        int x = index / yStride;
        int y = (index % yStride) / zStride;
        int z = index % zStride;
        return {x, y, z};
    }
    /**
     * Generates the dd for cic weighting.
     * @param point A point in the real particle space
     * @returns the cell fraction for distributing weight across the local nodes.
     */
    vec3<double> getCellFraction(vec3<double> point) const {
        vec3<double> g = realToGrid(point);
        vec3<int> cell = gridToCell(g);

        vec3<double> cellOrigin = vec3<double>(cell) * d;
        return (g - cellOrigin) / d;
    }
};