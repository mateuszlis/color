cmake_minimum_required (VERSION 2.6)
option(BUILD_TESTS "Build all of unit tests." FALSE)
project (color)

add_subdirectory(CMake)
set(Boost_USE_STATIC_LIBS   ON)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

FIND_PACKAGE( Boost 1.38 COMPONENTS regex REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )


# Set default build type
message(STATUS "${PROJECT_NAME}: Build type - ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -fstack-protector-all -Wall -Wextra --std=gnu++11 ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -O3")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_BINARY_DIR}")
set (BASE_SRC_DIR "${CMAKE_SOURCE_DIR}/src")

file(COPY
        ${CMAKE_CURRENT_SOURCE_DIR}/data/color.conf
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/color.conf
   DESTINATION /etc/
   COMPONENT configuration)

set(MAN_PATH $ENV{MANPATH})
if( "${MAN_PATH}" STREQUAL "" )
    set(MAN_PATH "/usr/man")
endif()

message(STATUS "Manual path ${MAN_PATH}")
if (BUILD_TESTS)
    # setup testing
    enable_testing()
    add_subdirectory(tests)
endif() 


install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/color.1
   DESTINATION ${MAN_PATH}/man1
   COMPONENT manual)

add_subdirectory(src)
