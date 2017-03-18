function(link_portaudio PROJ_NAME)
	# Added static/dynamic option
	if(TARGET portaudio)
		message(STATUS "PORTAUDIO already included")
	else()
		message(STATUS "PORTAUDIO not found, building from source")
		execute_process(COMMAND git submodule update --init -- vendor/portaudio
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_QUIET)
		execute_process(COMMAND git checkout pa_stable_v190600_20161030
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/portaudio OUTPUT_QUIET)
		set(PORTAUDIO_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vendor/portaudio/include CACHE PATH "portaudio include directory" FORCE)
		add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/portaudio EXCLUDE_FROM_ALL)
	endif()
	add_dependencies(${PROJ_NAME} portaudio_static)
	target_link_libraries(${PROJ_NAME} portaudio_static)
	target_include_directories(${PROJ_NAME} PRIVATE ${PORTAUDIO_INCLUDE_DIRS})
endfunction()
