cmake_minimum_required(VERSION 3.4)
project(flexy)

set(RUN_NAME ${PROJECT_NAME})
set(RUN_ARGS ${CMAKE_SOURCE_DIR}/assets/config.lua)
set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

file(GLOB_RECURSE CPP_FILES ${CMAKE_SOURCE_DIR}/src/flexy/*.cpp)

add_executable(${PROJECT_NAME} ${CPP_FILES})
add_dependencies(${PROJECT_NAME} sol2 flxr logger)

target_link_libraries (${PROJECT_NAME} logger)
if(NOT WIN32)
	target_link_libraries (${PROJECT_NAME} stdc++fs)
endif(NOT WIN32)
target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS} flxr)
link_zlib(${PROJECT_NAME})
link_lua(${PROJECT_NAME})

include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/execute.cmake)
