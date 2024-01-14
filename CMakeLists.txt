# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("ipaddress")

option(IPADDRESS_BUILD_SHARED "Build shared library, when off builds static library, currently only static is supported!" OFF)
option(IPADDRESS_BUILD_BENCHMARK "Build benchmarks" OFF)
option(IPADDRESS_BUILD_EXAMPLES "Build examples" OFF)
option(IPADDRESS_BUILD_UNIT_TEST "Build unit tests" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Include sub-projects.
add_subdirectory ("ipaddress")
