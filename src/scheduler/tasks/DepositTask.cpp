// deposits all the masses onto the 8 grid points in the cell
#include <stdexcept>
#include <cassert>
#include <cmath>

#include <vector>
#include <array>
#include <unordered_map>

#include <cstring>
#include <cstdint>

#include "tasks/Task.cpp"
#include "ParticleView.hpp"
#include "vec3.hpp"
#include "SimCTX.hpp"

struct DepositResult{
    std::array<int64_t, 8> totalMass{};

    //valid after run()
    std::array<int, 8> globalNodeIndices{};
};
/**
 * @invariant all particles belong to the same cell
 * @invariant at least one particle
 */
struct DepositTask : Task<DepositTask, DepositResult>{
    // an array that stores local node index-> global node index. Used for reduction.
    // values are implicitly mapped based on order of operations.
    std::vector<ParticleView> particleViews;
    DepositResult r;
    
    
    /**
     * Deposit task is a task that deposits all particles for  given cell onto a local-only mesh
     * 
     * @invariant task must contain only particles belonging to the same cell
     * @invariant this task must contain at least one particle
     * 
     * @param particleViews a vector of particle veiws to get utilized by the task
     * @param SimCTX a reference to the worker-local SimCTX object
     * 
     */
    DepositTask(std::vector<ParticleView> particleViews_)
        : particleViews(std::move(particleViews_))
    {
        if(particleViews.empty()){
            throw std::logic_error("DepositTask constructed with no particles");
        }
    }
    /**
     * Deposits all particles in this task onto the local grid.
     */
    void runImpl(const SimCTX& simCTX){
        const GridCTX& grid = simCTX.grid;

        const vec3<int> base = grid.locate(particleViews.front().position).cell;
        
        for(int i = 0; i < 8; i++){
            const vec3<int> c{base.x + ((i >> 2)& 1), base.y + ((i >> 1 ) & 1), base.z + (i & 1)};
            r.globalNodeIndices[i] = grid.cellToIndex(c);
        }
        r.totalMass = {};

        for(const ParticleView& pv : particleViews){
            const GridCTX::Located loc = grid.locate(pv.position);
            // ensure the particle lies within the cell for this task
            assert(loc.cell.x == base.x && loc.cell.y == base.y && loc.cell.z == base.z);
            const double wx[2] = {1.0 - loc.frac.x, loc.frac.x};
            const double wy[2] = {1.0 - loc.frac.y, loc.frac.y};
            const double wz[2] = {1.0 - loc.frac.z, loc.frac.z};

            //convert to int64 for better atomic behavior for reduction
            const double m = pv.mass * simCTX.scale;
            const std::int64_t total = std::llround(m);

            std::int64_t placed = 0;
            for(int i = 0; i < 7; i++){
                const std::int64_t v = std::llround(wx[i >> 2 & 1] * wy[(i >> 1) & 1] * wz[i & 1] * m);
                r.totalMass[i] += v;
                placed += v;
            }
            // apply leftover mass to the 8th corner for conservation of mass
            r.totalMass[7] = total - placed;

        }
    }
    const DepositResult getResultImpl() const {
        return r;
    }
};
using SparseGrid = std::unordered_map<int, std::int64_t>; // global node index -> node mass
struct LocalDepositReduction : TaskReduction<LocalDepositReduction, DepositResult, SparseGrid> {
    /**
     * Implementing the CRTP template's accumulateImpl
     */
    SparseGrid sparseGrid;

    void accumulateImpl(const DepositResult& result){
        for(int i = 0; i < 8; i++){
            sparseGrid[result.globalNodeIndices[i]] += result.totalMass[i];
        }
    }
    const SparseGrid& getResultImpl() const { 
        return sparseGrid; 
    };
};

struct InPlaceGlobalDepositReduction: TaskReduction<InPlaceGlobalDepositReduction, SparseGrid, void>{
    std::vector<float>& massMesh;
    double invScale;
    /** 
     * MESH MUST BE ZEROED BEFORE ANY OPERATION
     * @param massMesh The flattened mesh from the MassMesh object
     * @param simCTX The custom SimCTX object provided by the simulation object. 
     */
        InPlaceGlobalDepositReduction(std::vector<float>& massMesh_, const SimCTX& ctx)
        : massMesh(massMesh_), invScale(1.0 / ctx.scale) {}

    void accumulateImpl(const SparseGrid& sparseGrid) {
        for (const auto& [globalIndex, mass] : sparseGrid) {
            massMesh[globalIndex] += static_cast<double>(mass) * invScale;
        }
    }
    void getResultImpl() const {} 
};

/**
 * Accumulates in int64 then converts the final result 
 * Cost one int64 per node
 * to utilize correctly, call accumulate() and once it's finished, call finalize() as it applies directly to the global mesh
 */
struct ReproducibleGlobalDepositReduction
    : TaskReduction<ReproducibleGlobalDepositReduction, SparseGrid, void> {
    std::vector<std::int64_t>& intMesh;
 
    explicit ReproducibleGlobalDepositReduction(std::vector<std::int64_t>& intMesh_)
        : intMesh(intMesh_) {}
 
    void accumulateImpl(const SparseGrid& sparseGrid) {
        for (const auto& [globalIndex, mass] : sparseGrid) {
            std::atomic_ref<std::int64_t>(intMesh[globalIndex]).fetch_add(mass, std::memory_order_relaxed);
        }
    }
    void getResultImpl() const {}
    
    void finalize(std::vector<float>& massMesh, const SimCTX& ctx) const {
        assert(massMesh.size() == intMesh.size());
        const double invScale = 1.0 / ctx.scale;
        for (std::size_t i = 0; i < intMesh.size(); ++i) {
            massMesh[i] = static_cast<float>(static_cast<double>(intMesh[i]) * invScale);
        }
    }
};