cmake_minimum_required(VERSION 3.4)
project(engine)

set(RUN_NAME ${PROJECT_NAME})
set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

file(GLOB_RECURSE CPP_FILES ${CMAKE_SOURCE_DIR}/src/raid/*.cpp)
if(ANDROID)
	list(REMOVE_ITEM CPP_FILES ${CMAKE_SOURCE_DIR}/src/raid/platform/glfw/glfw.cpp)
else()
	list(REMOVE_ITEM CPP_FILES ${CMAKE_SOURCE_DIR}/src/raid/platform/android/android.cpp)
endif()

link_glm(${PROJECT_NAME})

if(NOT ANDROID)
	add_executable(${PROJECT_NAME} ${CPP_FILES})
else()
	add_library(${PROJECT_NAME} SHARED ${CPP_FILES})
endif()
add_dependencies(${PROJECT_NAME} flxr logger)

if(NOT ANDROID)
	find_package (OpenGL REQUIRED)
	if (OPENGL_FOUND)
		include_directories(${OPENGL_INCLUDE_DIRS})
		target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
	endif (OPENGL_FOUND)
endif()

target_link_libraries (${PROJECT_NAME} logger)
if(NOT WIN32 AND NOT ANDROID)
	target_link_libraries (${PROJECT_NAME} stdc++fs)
endif(NOT WIN32 AND NOT ANDROID)
target_link_libraries (${PROJECT_NAME} flxr)

link_spirv_cross(${PROJECT_NAME})

link_freetype(${PROJECT_NAME})

if(NOT ANDROID)
	link_zlib(${PROJECT_NAME})
	link_glew(${PROJECT_NAME})
	link_glfw(${PROJECT_NAME})
else()
	add_library(app-glue STATIC /home/tim/android-ndk/sources/android/native_app_glue/android_native_app_glue.c)
	target_link_libraries (${PROJECT_NAME} z log app-glue GLESv3 android EGL)
	target_include_directories(${PROJECT_NAME} PRIVATE /home/tim/android-ndk/sources/android/native_app_glue)
endif()


include(${CMAKE_SOURCE_DIR}/cmake/cmake-files/execute.cmake)
