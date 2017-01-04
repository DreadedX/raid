cmake_minimum_required(VERSION 3.4)
project(shader)

set(RUN_NAME ${PROJECT_NAME})
set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

file(GLOB_RECURSE CPP_FILES ${CMAKE_SOURCE_DIR}/src/plugins/shader/*.cpp)

add_library(${PROJECT_NAME} SHARED ${CPP_FILES})
# add_executable(${PROJECT_NAME} ${CPP_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "plugin_")
add_dependencies(${PROJECT_NAME} logger flexy)

target_link_libraries(${PROJECT_NAME} logger)
if(NOT WIN32)
	target_link_libraries (${PROJECT_NAME} stdc++fs)
endif(NOT WIN32)

link_shaderc(${PROJECT_NAME})
link_spirv_cross(${PROJECT_NAME})

include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/execute.cmake)
