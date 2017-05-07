cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

project(trigger_bdt_tool)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(run_bdt ${SOURCES})
