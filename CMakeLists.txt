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

# --- Add gprof profiling flags ---
# Add -pg flag for compiling and linking
target_compile_options(psb_algorithm PRIVATE -pg)
target_link_options(psb_algorithm PRIVATE -pg)
# --- End gprof flags ---

# Enable OpenMP (optional, for parallelization)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(psb_algorithm PUBLIC OpenMP::OpenMP_CXX)
endif()

# --- Link MPI Libraries (Ensure this is present if using MPI) ---
find_package(MPI REQUIRED)
if(MPI_FOUND)
    target_link_libraries(psb_algorithm PRIVATE MPI::MPI_CXX)
else()
    message(FATAL_ERROR "MPI not found, but required.")
endif()
# --- End MPI Linking ---