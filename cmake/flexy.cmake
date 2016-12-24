cmake_minimum_required(VERSION 3.4)
project(flexy)

set(RUN_NAME ${PROJECT_NAME})
set(RUN_ARGS ${CMAKE_SOURCE_DIR}/assets/config.lua)
set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

file(GLOB_RECURSE CPP_FILES ${CMAKE_SOURCE_DIR}/src/flexy/*.cpp)

add_executable(${PROJECT_NAME} ${CPP_FILES})
target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS} flxr)

find_package (ZLIB REQUIRED)
if (ZLIB_FOUND)
	include_directories(${ZLIB_INCLUDE_DIRS})
	target_link_libraries (${PROJECT_NAME} ${ZLIB_LIBRARIES})
endif (ZLIB_FOUND)
# find_package (Lua REQUIRED)
# if (Lua_FOUND)
	# include_directories(${LUA_INCLUDE_DIR})
	# target_link_libraries (${PROJECT_NAME} ${LUA_LIBRARIES})
if (UNIX)
	target_link_libraries (${PROJECT_NAME} /usr/lib64/liblua.so.5.3)
elseif(WIN32)
	target_link_libraries (${PROJECT_NAME} /usr/i686-w64-mingw32/lib/liblua53.dll.a)
endif()
# endif (Lua_FOUND)

target_link_libraries (${PROJECT_NAME} logger)
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
