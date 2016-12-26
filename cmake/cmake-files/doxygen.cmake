cmake_minimum_required(VERSION 3.4)
project(docs)

find_package(Doxygen)
if(DOXYGEN_FOUND)
	file(GLOB_RECURSE DOXYGEN_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
	string(REPLACE ".cpp" ".cpp " DOXYGEN_INPUT ${DOXYGEN_INPUT})
	string(REPLACE ".h" ".h " DOXYGEN_INPUT ${DOXYGEN_INPUT})
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/in-files/doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doxyfile @ONLY)
	add_custom_target(docs
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
endif(DOXYGEN_FOUND)

