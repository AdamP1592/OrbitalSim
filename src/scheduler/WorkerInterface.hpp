#pragma once
#include "Worker.hpp"
template<typename T>
struct WorkerInterface{
    Worker<T>& root;
    WorkerInterface(Worker<T>& w){
        root = w;
    }
    stealTask(){
        // REPLACE WITH NETWORKED TASK STEALING LOGIC WHEN PORTING TO CLUSTER
        return root.stealTask();
    }


}