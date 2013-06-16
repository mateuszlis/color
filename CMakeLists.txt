cmake_minimum_required (VERSION 2.6)
option(BUILD_TESTS "Build all of unit tests." FALSE)
project (color)



# Set default build type
message(STATUS "${PROJECT_NAME}: Build type - ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -fstack-protector-all -Wall -Wextra --std=gnu++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNO_DEBUG -DNDEBUG -fPIC ")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_BINARY_DIR}")
set (BASE_SRC_DIR "${CMAKE_SOURCE_DIR}/src")

# we will store variables from CMAKE in generated file

if (BUILD_TESTS)
    # setup testing
    enable_testing()
    add_subdirectory(tests)
endif() 

add_subdirectory(src)
