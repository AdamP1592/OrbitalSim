#pragma once
#include <vector>
#include <thread>
#include <stdexcept>
#include <random>
#include <mutex>
#include <atomic>
#include "tasks/DepositTask.hpp"
template <typename T>
struct Pool{
    std::vector<T> tasks;
    std::mutex mtx;
    std::atomic<size_t> cost{0};
};
template <typename T>
struct Schedule{
    std::vector<Pool<T>> pools;
    std::mt19937 rng;
    std::vector<SimCTX> simCTXs;
    size_t maxSize;
    bool splitting;
    /**
     * A semi-generic scheduler template that enforces uniform task-type for the schedule
     * @param poolCount the number of pools you want to distribute tasks to
     * @todo Implement task splitting
     * @param maxSize (UNIMPLEMENTED) the max number of resources the task can operate with before splitting
     * @param splitting (UNIMPLEMENTED) a bool to enable splitting
     */
    Schedule(std::vector<SimCTX> simCTXs_, size_t poolCount, size_t maxSize_ = -1, bool splitting_ = false):
            simCTXs(std::move(simCTXs_)), maxSize(maxSize_), splitting(splitting_){
        pools.resize(poolCount);
    }
    /**
     * A task-specific constructor that takes in the fields necessary for the task.
     * @param pvs a vector of particle views from the ParticleStorage object
     */
    void createDepositTask(std::vector<ParticleView> pvs){
        // replace this with task splitting later || splitting && static_cast<size_t>(maxSize) < pvs.size())
        if(splitting){
            throw std::logic_error("Automatic task splitting is not implemented in this version");
        }
        size_t poolIdx = pickAndReserve(pvs.size());
        DepositTask d = DepositTask(std::move(pvs), simCTXs[poolIdx]);

        std::lock_guard<std::mutex> lock(pools[poolIdx].mtx);
        pools[poolIdx].tasks.push_back(std::move(d));

    }
    /**
     * A function that selects which pool to add a resource tool based on based on Mitzenmacher's load balancing
     * @param taskCost an estimate on how much processing the task will have to do(mostly just resource count)
     */
    size_t pickAndReserve(size_t taskCost) {
        // a random sample of 2 possible entries and selecting the best of the two 
        // will yield an almost the same balance as checking all pools
        
        // grabs 2 random pools
        std::uniform_int_distribution<size_t> dist(0, pools.size() - 1);
        size_t a = dist(rng), b = dist(rng);

        // chooses the lowest resource pool
        size_t chosen = (
            pools[a].cost.load(std::memory_order_relaxed) <
            pools[b].cost.load(std::memory_order_relaxed)) ? a : b;


        // adds to the cost
        pools[chosen].cost.fetch_add(taskCost, std::memory_order_relaxed);

        return chosen;
    }
};
/**
 * A free helper function that recieves a SimCTX object and creates n copies to be used by the Schedule
 * @param ctx A const simCTX object to copy
 * @param poolCount the number of resource pools the schedule will use.
 * @returns a vector of SimCTXs to get used for schedules that require the SimCTX object
 */
std::vector<SimCTX> buildSimCTXs(const SimCTX ctx, size_t poolCount){
    std::vector<SimCTX> simCTXs;
    simCTXs.resize(poolCount, ctx);
    return simCTXs;
}