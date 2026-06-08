#pragma once
#include <vector>
#include <atomic>
#include <mutex>
#include <optional>
template <typename T>
struct Worker{
    private:
        std::mutex m;
        T tasks;
        // REPLACE T WHEN TASKS ARE FULLY IMPLEMENTED
        std::vector<uint32_t> taskIndices;

        // front and back ind are atomic because they're used in access directly.
        std::atomic<uint32_t> front;
        std::atomic<uint32_t> back;


        std::vector<uint32_t> stolenBuffer;
        size_t stolenIndex = 0; 

        std::vector<WorkerInterface*> workers;
    public:
    
        Worker(std::vector<T>& taskIndices_){
            front = 0;
            back = taskIndices_.size();
            taskIndices = taskIndices_;
        }
        /**
         * Gets the next taskIndex available if it exists.
         * @returns {uint32_t} index
         */
        uint32_t getNextIndex(){
            uint32_t claimed = front.fetch_add(1);

            if(back.load() <= claimed){
                front.fetch_sub(1);
                return std::nullopt;
            }
            
            return std::move(taskIndices[claimed]);
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
            if(claimed <= front.load() + 1){
                // add 1 to back index because this wasn't a valid grab, return null
                back.fetch_add(1);
                return std::nullopt;
            }
            
            return std::move(taskIndices[claimed]);
        }
        void performTask(T task){
            //perform a specific given task
        }
        void performTask(uint32_t index){
            // perform specific task at the given index. 
        }
        /**
         * Performs the next task in the resource pool
         * @returns {bool} taskCompleted
         */
        bool performNextTask(){
            uint32_t nextIndex = getNextIndex();
            if(nextIndex == -1){
                return false;
            }
            performTask(nextIndex);
            return true;
        }
        void performAllTasks(){
            while(performNextTask());
        }

        /**
         * Performs all task within the stolenBuffer
         */
        void performStolenTasks(){
            while(stolenIndex < stolenBuffer.size()){
                performTask(stolenBuffer[stolenIndex]);
                stolenIndex++;
            }
        }
        void peformStolenTask(){
            if(stolenIndex < stolenBuffer.size()){
                perfromTask(stolenBuffer[stolenIndex]);
                stolenIndex++;
            }
            
        }

        /**
         * Steals task from the first worker with a free task
         */
        void stealTask(){
            for(WorkerInterface* w: workers){
                auto task = w->steal();
                if(task){
                    taskIndices.push_back(task);
                    break;
                }
            }
        }
        void stealNTasks(uint32_t n){
            uint32_t count = 0;

            int workerIndex = 0;
            int workerSize = workers.size();

            int numWorkersSinceLastTask = 0;
            while(count < n && numWorkersSinceLastTask < workerSize){
                auto task = workers[workerIndex]->steal();
                if(task){
                    count++;
                    numWorkersSinceLastTask = 0;
                    stolenBuffer.push_back(task);
                }else{
                    numWorkersSinceLastTask++;
                }  
                workerIndex = (workerIndex + 1) % workerSize;
            }

        }
        /**
         * RESIZES TASKS. DO NOT USE IF YOU MANIPULATE TASKS EXTERNALLY DURING TASK PERFORMANCE
         */
        void reset(){
            front = 0;
            stolenIndex = 0;

            // make room for the buffer
            taskIndices.resize(back + stolenBuffer.size());

            // transfer the buffer into the task indices
            std::copy(stolenBuffer.begin(), stolenBuffer.end(), taskIndices.begin() + back);
            back+= stolenBuffer.size();

            //cleanup
            stolenBuffer.clear();
            
        }

};