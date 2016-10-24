cmake_minimum_required(VERSION 3.4)
project(png2bin)

set(NAME_ ${PROJECT_NAME})
set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

file(GLOB_RECURSE CPP_FILES ${CMAKE_SOURCE_DIR}/src/png2bin/*.cpp)

add_library(${PROJECT_NAME} SHARED ${CPP_FILES})

find_package (ZLIB REQUIRED)
if (ZLIB_FOUND)
	include_directories(${ZLIB_INCLUDE_DIRS})
	target_link_libraries (${PROJECT_NAME} ${ZLIB_LIBRARIES})
endif (ZLIB_FOUND)

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
