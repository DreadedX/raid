cmake_minimum_required(VERSION 3.4)
project(ycm)

if (UNIX)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/run.sh.in ${CMAKE_CURRENT_BINARY_DIR}/run-${NAME_}.sh @ONLY)
	execute_process(COMMAND chmod +x ${CMAKE_CURRENT_BINARY_DIR}/run-${NAME_}.sh)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/debug.sh.in ${CMAKE_CURRENT_BINARY_DIR}/debug-${NAME_}.sh @ONLY)
	execute_process(COMMAND chmod +x ${CMAKE_CURRENT_BINARY_DIR}/debug-${NAME_}.sh)
endif (UNIX)
