cmake_minimum_required(VERSION 3.4)
project(engine)

set(RUN_NAME ${PROJECT_NAME})
set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

file(GLOB_RECURSE CPP_FILES ${CMAKE_SOURCE_DIR}/src/raid/*.cpp)

add_executable(${PROJECT_NAME} ${CPP_FILES})

find_package (OpenGL REQUIRED)
if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
    target_link_libraries (${PROJECT_NAME} ${OPENGL_LIBRARIES})
endif (OPENGL_FOUND)
find_package (GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries (${PROJECT_NAME} ${GLEW_LIBRARIES})
endif (GLEW_FOUND)
find_package (PkgConfig REQUIRED)

if(UNIX)
pkg_search_module (GLFW REQUIRED glfw3)
if (GLFW_FOUND)
    include_directories(${GLFW_INCLUDE_DIRS})
	target_link_libraries (${PROJECT_NAME} ${GLFW_LIBRARIES})
endif (GLFW_FOUND)
endif(UNIX)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} glew32 glfw3 opengl32 imm32)
endif(WIN32)

find_package (ZLIB REQUIRED)
if (ZLIB_FOUND)
	include_directories(${ZLIB_INCLUDE_DIRS})
	target_link_libraries (${PROJECT_NAME} ${ZLIB_LIBRARIES})
endif (ZLIB_FOUND)
target_link_libraries (${PROJECT_NAME} flxr logger)
if(NOT WIN32)
	target_link_libraries (${PROJECT_NAME} stdc++fs)
endif(NOT WIN32)

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

include(${CMAKE_SOURCE_DIR}/cmake/execute.cmake)
