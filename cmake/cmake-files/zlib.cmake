function(link_zlib PROJ_NAME)
	find_package (ZLIB REQUIRED)

	if (ZLIB_FOUND AND NOT WIN32 AND NOT BUILD_DEPENDENCIES)
		message(STATUS "ZLIB found")
		target_include_directories(${PROJ_NAME} PRIVATE ${ZLIB_INCLUDE_DIRS})
	target_link_libraries(${PROJ_NAME} ${ZLIB_LIBRARIES})
	else()
		# Added static/dynamic option
		if(TARGET zlib OR TARGET zlibstatic)
			message(STATUS "ZLIB already included")
		else()
			message(STATUS "ZLIB not found, building from source")
			execute_process(COMMAND git submodule update --init -- vendor/zlib
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_QUIET)
			execute_process(COMMAND git checkout v1.2.8
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/zlib OUTPUT_QUIET)
			execute_process(COMMAND git am ${CMAKE_CURRENT_SOURCE_DIR}/cmake/patches/zlib001.patch
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/zlib)
			set(ZLIB_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vendor/zlib CACHE PATH "zlib include directory" FORCE)
			add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/zlib EXCLUDE_FROM_ALL)
		endif()
		add_dependencies(${PROJ_NAME} zlibstatic)
		target_link_libraries(${PROJ_NAME} zlibstatic)
		target_include_directories(${PROJ_NAME} PRIVATE ${ZLIB_INCLUDE_DIRS})
	endif()
endfunction()
