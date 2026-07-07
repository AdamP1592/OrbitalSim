#include "Worker.cpp"
struct WorkerInterface{
    Worker& root;
    WorkerInterface(Worker& w){
        root = w;
    }
    stealTask(){
        return root.stealTask();
    }


}