# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(ParallelSocialBehavior VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for header files
include_directories(include)

# Source files
file(GLOB SOURCES
    src/*.cpp
    src/utils/*.cpp
)

# Add executable target
add_executable(psb_algorithm ${SOURCES})

# Enable warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(psb_algorithm PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Enable OpenMP for parallelization
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    target_link_libraries(psb_algorithm PUBLIC OpenMP::OpenMP_CXX)
endif()

# Enable MPI for distributed computing
find_package(MPI REQUIRED)
if(MPI_CXX_FOUND)
    target_link_libraries(psb_algorithm PUBLIC MPI::MPI_CXX)
    message(STATUS "MPI support enabled")
endif()