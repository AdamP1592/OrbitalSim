# Scheduler

This is a scheduler built for threaded compatibility and hpc extensibility with autobalancing via worker stealing

## Structure

Structure is relatively simple. Each Scheduler stores n task pools where N is specified at the start. Each pool stores a series of tasks with no overlapping written resources. Scheduler class has two sublclasses, a TaskSchedule, and a ScheduleOfSchedules, but all tasks are initiated with scheduler.runTask(). Upon completion of all tasks, the thief will use a provided interface StealTask for each other worker to find the next available resource. If none are found, the process is done. This is a persistent mutation of the pool.

### WORKERS

- Inputs in constructor:
    - an interface for every other worker to get used with stealing.
    - a resource pool of tasks
- Stores: 
    - an atomic frontIndex
    - a mutex-locked tailIndex
- Functions:
    - stealTask() - uses mutex locked variable for tail steailing
    - runTask() - completes a single task from the pool
    - runTasks() - threaded, completes all tasks in the pool

### TASKS

Basic Requirements: 

- A task takes in a series of inputs necessary for a given task, has a void task.run function 

### DEPOSIT TASK

Deposit masses onto a grid.

- Stores 8 floats, 1 for each corner of the cell. 
- Inputs in constructor:
    - a ParticleView vector for all the particles in that specific cell, 
    - a const reference to a lightweight GridContext object for translating between coordinate types. 

- Run deposits all particles in the vector onto those 8 floats as though they were the true grid

- Reduce by collecting adjacent cells.

### MOVE TASK

Extract acceleration from accel field, apply it to the particles based on leapfrog, move the particles, flag which particles migrated to a new cell. 

- Inputs in constructor:
    - a ParticleView vector for all the particles in that specific cell, 
    - a const reference to a lightweight GridContext object for translating between coordinate types. 
    - a const reference to the accelfield

- Run: read description

- No reduction necessary, but migration is done right after moving.
