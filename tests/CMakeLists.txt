add_library(LogHelper INTERFACE)
target_include_directories(LogHelper INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
add_subdirectory(math)
add_subdirectory(datastructures)
