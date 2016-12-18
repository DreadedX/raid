cmake_minimum_required(VERSION 3.4)
project(png)

set(NAME_ ${PROJECT_NAME})
set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

file(GLOB_RECURSE CPP_FILES ${CMAKE_SOURCE_DIR}/src/plugins/png/*.cpp)

add_library(${PROJECT_NAME} SHARED ${CPP_FILES})

find_package(PNG REQUIRED)
if(PNG_FOUND)
	include_directories(${PNG_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARIES})
endif(PNG_FOUND)

target_link_libraries(${PROJECT_NAME} logger)
target_link_libraries (${PROJECT_NAME} stdc++fs)

include(sugar_generate_warning_flags)
sugar_generate_warning_flags(
    target_compile_options
    target_properties
	ENABLE ALL
)
set_target_properties(
    engine
    PROPERTIES
    ${target_properties} # important: without quotes (properties: name, value, name, value, ...)
    COMPILE_OPTIONS
    "${target_compile_options}" # important: need quotes (one argument for COMPILE_OPTIONS)
)