add_library(Scheduler
    /Scheduler.cpp
    /Worker.cpp
    /WorkerInterface.cpp
    /tasks/Task.cpp


)
target_include_directories(Sheduler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)