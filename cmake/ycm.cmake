cmake_minimum_required(VERSION 3.4)
project(ycm)

if (UNIX)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ycm-config.sh.in ${CMAKE_CURRENT_BINARY_DIR}/ycm-config.sh @ONLY)
	execute_process(COMMAND chmod +x ${CMAKE_CURRENT_BINARY_DIR}/ycm-config.sh)
endif (UNIX)
