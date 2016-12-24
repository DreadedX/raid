cmake_minimum_required(VERSION 3.4)
project(execute)

if (UNIX)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/run.sh.in ${CMAKE_CURRENT_BINARY_DIR}/run-${RUN_NAME}.sh @ONLY)
	execute_process(COMMAND chmod +x ${CMAKE_CURRENT_BINARY_DIR}/run-${RUN_NAME}.sh)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/debug.sh.in ${CMAKE_CURRENT_BINARY_DIR}/debug-${RUN_NAME}.sh @ONLY)
	execute_process(COMMAND chmod +x ${CMAKE_CURRENT_BINARY_DIR}/debug-${RUN_NAME}.sh)
endif (UNIX)
if (WIN32)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/run-mingw32.sh.in ${CMAKE_CURRENT_BINARY_DIR}/run-${RUN_NAME}.sh @ONLY)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/run-mingw32.bat.in ${CMAKE_CURRENT_BINARY_DIR}/run-${RUN_NAME}.bat @ONLY)
	execute_process(COMMAND chmod +x ${CMAKE_CURRENT_BINARY_DIR}/run-${RUN_NAME}.sh)
	# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/debug-mingw32.sh.in ${CMAKE_CURRENT_BINARY_DIR}/debug-${RUN_NAME}.sh @ONLY)
	# execute_process(COMMAND chmod +x ${CMAKE_CURRENT_BINARY_DIR}/debug-${RUN_NAME}.sh)
endif (WIN32)
