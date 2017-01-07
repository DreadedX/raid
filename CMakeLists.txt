cmake_minimum_required(VERSION 3.4)
project(all)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

if(ANDROID)
     set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${my_cxx_flags}" )
     set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${my_cxx_flags}" )
 endif()

# Make sure there is no in source build
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
# set(CMAKE_DISABLE_SOURCE_CHANGES ON)

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
if(NOT ANDROID)
	set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/${SYSTEM_NAME}/bin)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${SYSTEM_NAME}/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
set(VENDOR_PATH ${CMAKE_CURRENT_BINARY_DIR}/vendor)

include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/spirv-cross.cmake)
if(NOT ANDROID)
	include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/sol2.cmake)
	include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/zlib.cmake)
	include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/glew.cmake)
	include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/glfw.cmake)
	include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/lua.cmake)
	include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/shaderc.cmake)

	include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/flexy.cmake)
	include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/plugin_png.cmake)
	include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/plugin_reference.cmake)
	include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/plugin_shader.cmake)
	include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/plugin_default.cmake)

	include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/doxygen.cmake)
	include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/ycm.cmake)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/logger.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/flxr.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/engine.cmake)

# Plugins
if(NOT ANDROID)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/sugar/Sugar)
include(sugar_generate_warning_flags)
sugar_generate_warning_flags(
    target_compile_options
    target_properties
	ENABLE ALL
)
if(NOT ANDROID)
	set_target_properties(
		logger flxr flexy engine png reference
		PROPERTIES
		${target_properties} # important: without quotes (properties: name, value, name, value, ...)
		COMPILE_OPTIONS
		"${target_compile_options}" # important: need quotes (one argument for COMPILE_OPTIONS)
		)
else()
	set_target_properties(
		logger flxr engine
		PROPERTIES
		${target_properties} # important: without quotes (properties: name, value, name, value, ...)
		COMPILE_OPTIONS
		"${target_compile_options}" # important: need quotes (one argument for COMPILE_OPTIONS)
		)
endif()
