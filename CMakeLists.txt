cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)

project(trigger_bdt_tool)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Get ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS Core RIO Hist Net Tree TreePlayer)
include(${ROOT_USE_FILE})

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})
include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(run_bdt ${SOURCES})
target_link_libraries(run_bdt ${ROOT_LIBRARIES})
