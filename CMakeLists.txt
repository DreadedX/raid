cmake_minimum_required(VERSION 3.4)
project(all)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

# Make sure there is no in source build
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED ON)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/sugar/Sugar)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/engine.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/flxr.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/flexy.cmake)

# Plugins
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/plugin_reference.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/doxygen.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ycm.cmake)
