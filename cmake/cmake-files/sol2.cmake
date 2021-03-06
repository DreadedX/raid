include(ExternalProject)

find_package(PythonInterp 3 REQUIRED)

set(SOL2_TAG v2.15.5)
set(SOL2_HPP ${CMAKE_SOURCE_DIR}/build/${SYSTEM_NAME}/vendor/include/sol.hpp)

project(sol2-download NONE)

ExternalProject_add(sol2
	PREFIX "${VENDOR_PATH}/tmp"
	SOURCE_DIR "${VENDOR_PATH}/sol2-src"
	BINARY_DIR "${VENDOR_PATH}/sol2-build"

    GIT_REPOSITORY "https://github.com/ThePhD/sol2.git"
    GIT_TAG ${SOL2_TAG}
	# COMMENT THIS OUT WHEN UPDATING TO NEW VERSION
	UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    # Generate the single header and put it in ${SOL2_HPP}
    # BINARY_DIR ${VENDOR_PATH}/src/sol2
    BUILD_COMMAND
            ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/build/${SYSTEM_NAME}/vendor/include/
        COMMAND
		${PYTHON_EXECUTABLE} ${VENDOR_PATH}/sol2-src/single.py -o "${SOL2_HPP}"
    # No install or test command for the library
    INSTALL_COMMAND ""
    TEST_COMMAND "")

# Conditionally turn on SOL_CHECK_ARGUMENTS if using Debug mode
if (CMAKE_BUILD_TYPE MATCHES "[Dd]ebug")
    # if (VERBOSE)
        message(STATUS "Turning on SOL_CHECK_ARGUMENTS in Debug mode.")
    # endif()
    add_definitions(-DSOL_CHECK_ARGUMENTS)
endif()

# Make sure sol2 is found as a system directory
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/build/${SYSTEM_NAME}/vendor/include)
