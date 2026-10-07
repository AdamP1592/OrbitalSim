#include "tasks/Task.cpp"
#include <stdexcept>
#include "vec3.hpp"
#include "ParticleStorage.hpp"
#include "physics/gravity/GridCTX.cpp"

struct CreateParticlesTask: Task<CreateParticlesTask, std::vector<size_t>> {
    ParticleStorage &ps;
    GridCTX gridCTX;
    std::vector<size_t> depositIndices;

    std::vector<vec3<double>> positions;
    std::vector<vec3<double>> velocities;

    std::vector<double> masses;
    std::vector<int> cellIndices;
    /**
     * @brief deposit task for batch deposits
     * 
     * @note All inputs must be ordered together and must have the same length.
     * 
     * @param _ps A reference to the root particle storage, must already be mutated to fit all deposit particles
     * @param _depositIndices A vector of all the deposits that belong to this task.
     * @param _positions A vector of all particle positions
     * @param _velocities A vector of all the velocities of these particles
     * @note For better performance, split tasks by adjacenct indices in the particle storage and then balanace load by particle count.
     */
    CreateParticlesTask(
        GridCTX _gridCTX, ParticleStorage &_ps, std::vector<size_t> _depositIndices, 
        std::vector<vec3<double>> _positions, std::vector<vec3<double>> _velocities,
        std::vector<double> _masses): 
        gridCTX(_gridCTX), ps(_ps), depositIndices(_depositIndices), 
        positions(_positions), velocities(_velocities), masses(_masses){
            size_t base = _depositIndices.size();
            // prevent a size mismatch once.
            if(positions.size() != base || velocities.size() != base ||
            masses.size() != base || _cellIndices.size() != base){
                std::range_error("Error: Size mismatch between input vectors");
            }

        }
    /**
     * @brief Creates the particles in ParticleStorage
     * 
     * Using a direct reference to storage and the given indices the task is allowed to touch, create n particles in storage
     */
    void runImpl(){
        int totalEntries = depositIndices.size();
        for(int i = 0; i < totalEntries; i++){
            int cellIndex = gridCTX.realToCell(positions[i]);
            ps.setParticle(depositIndices[i], positions[i], velocities[i], masses[i], cellIndex);
        }
    }
};
