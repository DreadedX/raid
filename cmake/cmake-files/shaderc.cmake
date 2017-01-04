function(link_shaderc PROJ_NAME)
	if(TARGET shaderc)
		message(STATUS "SHADERC already included")
	else()
		message(STATUS "SHADERC not found, building from source")
		execute_process(COMMAND git submodule update --init -- vendor/shaderc
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_QUIET)
		execute_process(COMMAND git clone https://github.com/google/googletest.git
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/shaderc/third_party OUTPUT_QUIET)
		execute_process(COMMAND git clone https://github.com/google/glslang.git
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/shaderc/third_party OUTPUT_QUIET)
		execute_process(COMMAND git clone https://github.com/KhronosGroup/SPIRV-Tools.git spirv-tools
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/shaderc/third_party OUTPUT_QUIET)
		execute_process(COMMAND git clone https://github.com/KhronosGroup/SPIRV-Headers.git spirv-tools/external/spirv-headers
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/shaderc/third_party OUTPUT_QUIET)
		# execute_process(COMMAND git checkout 5.3.2
		# 	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/shaderc OUTPUT_QUIET)
		set(SHADERC_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vendor/shaderc/include ${CMAKE_CURRENT_SOURCE_DIR}/vendor/shaderc/glslc/src ${CMAKE_CURRENT_SOURCE_DIR}/vendor/shaderc/libshaderc_util/include CACHE PATH "shaderc include directory" FORCE)
		set( CMAKE_POSITION_INDEPENDENT_CODE ON )
		add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/shaderc EXCLUDE_FROM_ALL)
	endif()
	add_dependencies(${PROJ_NAME} shaderc shaderc_util glslc)
	target_link_libraries(${PROJ_NAME} shaderc shaderc_util glslc)
	target_include_directories(${PROJ_NAME} PRIVATE ${SHADERC_INCLUDE_DIRS})
endfunction()
