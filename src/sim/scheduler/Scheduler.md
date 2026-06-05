# Scheduler

This is a scheduler built for threaded compatibility and hpc extensibility with autobalancing via worker stealing

## Structure

Structure is relatively simple. Each Scheduler stores n task pools where N is specified at the start. Each pool stores a series of tasks with no overlapping written resources. Scheduler class has subclasses for each kind of task, but all tasks are initiated with scheduler.runTask(). Upon completion of all tasks, the scheduler will steal a task from another pool to give to the completed worker. This is a persistent mutation of the pool.

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

- Inputs in constructor are the same as Deposit Task
