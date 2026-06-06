#pragma once
#include <vector>
#include <atomic>
#include <mutex>
#include <optional>
#include "CellWrapper.hpp"
struct Worker{
    std::mutex m;
    std::vector<CellWrapper>& cells;
    std::atomic<uint32_t> front;
    std::atomic<uint32_t> back;

    std::vector<Worker*> 
    
    Worker(std::vector<CellWrapper>& cells_){
        front = 0;
        back = cells_.size();
        cells = cells_;
    }
    /**
     * Gets the next taskIndex available if it exists.
     * @returns {uint32_t} index
     */
    uint32_t getNextIndex(){
        uint32_t claimed = front.fetch_add(1);

        if(back <= claimed){
            front.fetch_sub(1);
            return -1;
        }
        
        return claimed;
    }

    /**
     * Extracts the last task from the task pool 
     * 
     * @returns {std::optional} CellWrapper.
     */
    std::optional<CellWrapper> getLast(){

        // claim the current back
        uint32_t claimed = back.fetch_sub(1);

        // 1 extra cell is padding in case this worker is about to start it's next task
        if(claimed <= front + 1){
            // add 1 to back index because this wasn't a valid grab, return null
            back.fetch_add(1);
            return std::nullopt;
        }
        
        return std::move(cells[back]);
    }
    void performTask(uint32_t index){
        // perform specific task at the given index. 
    }
    bool performNextTask(){
        uint32_t nextIndex = getNextIndex();
        performTask(nextIndex);
    }

};