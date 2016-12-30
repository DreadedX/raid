cmake_minimum_required(VERSION 3.4)
project(flxr)

set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

file(GLOB_RECURSE CPP_FILES ${CMAKE_SOURCE_DIR}/src/flxr/*.cpp)

add_library(${PROJECT_NAME} STATIC ${CPP_FILES})
add_dependencies(${PROJECT_NAME} logger)

if(NOT ANDROID)
link_zlib(${PROJECT_NAME})
else()
target_link_libraries (${PROJECT_NAME} z)
endif()
