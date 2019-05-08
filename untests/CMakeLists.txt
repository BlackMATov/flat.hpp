# 3.11 version is required for `FetchContent`
cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(flat.hpp.untests)

set(CATCH_BUILD_TESTING OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/catch2)

FetchContent_GetProperties(catch2)
if(NOT catch2_POPULATED)
    FetchContent_Populate(catch2)
    add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
endif()

file(GLOB UNTESTS_SOURCES "*.cpp" "*.hpp")
add_executable(${PROJECT_NAME} ${UNTESTS_SOURCES})
target_link_libraries(${PROJECT_NAME}
    Catch2
    flat.hpp)
add_test(${PROJECT_NAME} ${PROJECT_NAME})