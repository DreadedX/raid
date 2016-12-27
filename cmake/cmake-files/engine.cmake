cmake_minimum_required(VERSION 3.4)
project(engine)

set(RUN_NAME ${PROJECT_NAME})
set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

file(GLOB_RECURSE CPP_FILES ${CMAKE_SOURCE_DIR}/src/raid/*.cpp)

add_executable(${PROJECT_NAME} ${CPP_FILES})
add_dependencies(${PROJECT_NAME} flxr logger)

find_package (OpenGL REQUIRED)
if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
endif (OPENGL_FOUND)

target_link_libraries (${PROJECT_NAME} logger)
if(NOT WIN32)
	target_link_libraries (${PROJECT_NAME} stdc++fs)
endif(NOT WIN32)
target_link_libraries (${PROJECT_NAME} flxr)
link_zlib(${PROJECT_NAME})
link_glew(${PROJECT_NAME})
link_glfw(${PROJECT_NAME})

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

include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/execute.cmake)
