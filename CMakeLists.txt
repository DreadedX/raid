cmake_minimum_required(VERSION 3.4)
project(all)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

# Make sure there is no in source build
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

if(NOT CMAKE_BUILD_TYPE) 
	set(CMAKE_BUILD_TYPE "Debug")
	message(STATUS "Debug build")
else()
	message(STATUS "${CMAKE_BUILD_TYPE} build")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED ON)

string(TOLOWER "${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}" SYSTEM_NAME)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/${SYSTEM_NAME}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${SYSTEM_NAME}/lib)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(VENDOR_PATH ${CMAKE_CURRENT_BINARY_DIR}/vendor)

include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/sugar/Sugar)
include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/sol2.cmake)

include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/engine.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/logger.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/flxr.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/flexy.cmake)

# Plugins
include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/plugin_reference.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/plugin_png.cmake)

include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/doxygen.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/ycm.cmake)
