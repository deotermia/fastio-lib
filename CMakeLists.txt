cmake_minimum_required(VERSION 3.15)
project(fastio VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(fastio STATIC src/format.cpp src/io_utils.cpp)

add_executable(test_fastio test/test_basic.cpp)
target_link_libraries(test_fastio fastio)

add_executable(example examples/example.cpp)
target_link_libraries(example fastio)

add_executable(benchmark examples/benchmark.cpp)
target_link_libraries(benchmark fastio)