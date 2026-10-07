# Add .hpp for visibility in vscode
add_library(Scheduler
    Schedule.hpp
    Worker.hpp
    WorkerInterface.hpp
    tasks/Task.cpp

)
target_include_directories(Scheduler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(Scheduler math)
target_link_libraries(Scheduler orbital_core)