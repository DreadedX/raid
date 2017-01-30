function(link_glm PROJ_NAME)
	message(STATUS "GLM not found, building from source")
	execute_process(COMMAND git submodule update --init -- vendor/glm
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_QUIET)
	execute_process(COMMAND git checkout 0.9.8.4
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm OUTPUT_QUIET)
	set(GLM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm CACHE PATH "glm include directory" FORCE)
	# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm EXCLUDE_FROM_ALL)
	# target_include_directories(${PROJ_NAME} PRIVATE ${GLM_INCLUDE_DIRS})
	include_directories(${GLM_INCLUDE_DIRS})
endfunction()
