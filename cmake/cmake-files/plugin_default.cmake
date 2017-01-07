cmake_minimum_required(VERSION 3.4)
project(default)

set(NAME_ ${PROJECT_NAME})
set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

file(GLOB_RECURSE CPP_FILES ${CMAKE_SOURCE_DIR}/src/plugins/default/*.cpp)

add_library(${PROJECT_NAME} SHARED ${CPP_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "plugin_")
add_dependencies(${PROJECT_NAME} logger flexy)

target_link_libraries(${PROJECT_NAME} logger)
if(NOT WIN32)
	target_link_libraries (${PROJECT_NAME} stdc++fs)
endif(NOT WIN32)
