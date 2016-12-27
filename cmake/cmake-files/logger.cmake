cmake_minimum_required(VERSION 3.4)
project(logger)

set( CMAKE_POSITION_INDEPENDENT_CODE ON )

set(NAME_ ${PROJECT_NAME})
set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

file(GLOB_RECURSE CPP_FILES ${CMAKE_SOURCE_DIR}/src/logger.cpp)

add_library(${PROJECT_NAME} STATIC ${CPP_FILES})

include(sugar_generate_warning_flags)
sugar_generate_warning_flags(
    target_compile_options
    target_properties
	ENABLE ALL
)
set_target_properties(
	${PROJECT_NAME}
    PROPERTIES
    ${target_properties} # important: without quotes (properties: name, value, name, value, ...)
    COMPILE_OPTIONS
    "${target_compile_options}" # important: need quotes (one argument for COMPILE_OPTIONS)
)
