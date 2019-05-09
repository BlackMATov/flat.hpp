# 3.11 version is required for `FetchContent`
cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(flat.hpp.unbench)

#
# boost
#

find_package(Boost
    OPTIONAL_COMPONENTS container)

#
# google benchmark
#

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
    gbench
    GIT_REPOSITORY https://github.com/google/benchmark)

FetchContent_GetProperties(gbench)
if(NOT gbench_POPULATED)
    FetchContent_Populate(gbench)
    add_subdirectory(${gbench_SOURCE_DIR} ${gbench_BINARY_DIR})
endif()

#
# benchmark executable
#

file(GLOB UNBENCH_SOURCES "*.cpp" "*.hpp")
add_executable(${PROJECT_NAME} ${UNBENCH_SOURCES})

target_link_libraries(${PROJECT_NAME}
    benchmark_main
    flat.hpp)

if(Boost_CONTAINER_FOUND)
    target_link_libraries(${PROJECT_NAME}
        Boost::container)
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE BOOST_CONTAINER_FOUND)
endif()

add_test(${PROJECT_NAME} ${PROJECT_NAME})
