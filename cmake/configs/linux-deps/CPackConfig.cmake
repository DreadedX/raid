# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


SET(CPACK_BINARY_7Z "")
SET(CPACK_BINARY_BUNDLE "")
SET(CPACK_BINARY_CYGWIN "")
SET(CPACK_BINARY_DEB "")
SET(CPACK_BINARY_DRAGNDROP "")
SET(CPACK_BINARY_IFW "")
SET(CPACK_BINARY_NSIS "")
SET(CPACK_BINARY_OSXX11 "")
SET(CPACK_BINARY_PACKAGEMAKER "")
SET(CPACK_BINARY_PRODUCTBUILD "")
SET(CPACK_BINARY_RPM "")
SET(CPACK_BINARY_STGZ "")
SET(CPACK_BINARY_TBZ2 "")
SET(CPACK_BINARY_TGZ "")
SET(CPACK_BINARY_TXZ "")
SET(CPACK_BINARY_TZ "")
SET(CPACK_BINARY_WIX "")
SET(CPACK_BINARY_ZIP "")
SET(CPACK_BUILD_SOURCE_DIRS "/home/tim/Projects/cpp/raid;/home/tim/Projects/cpp/raid/cmake/configs/linux-deps")
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
SET(CPACK_COMPONENTS_ALL "Runtime;Library;Header;Data;Documentation;Example;Other")
SET(CPACK_COMPONENTS_ALL_SET_BY_USER "TRUE")
SET(CPACK_COMPONENT_DATA_DESCRIPTION "Application data. Installed into share/lua.")
SET(CPACK_COMPONENT_DATA_DISPLAY_NAME "lua Data")
SET(CPACK_COMPONENT_DOCUMENTATION_DESCRIPTION "Application documentation. Installed into share/lua/doc.")
SET(CPACK_COMPONENT_DOCUMENTATION_DISPLAY_NAME "lua Documentation")
SET(CPACK_COMPONENT_EXAMPLE_DESCRIPTION "Examples and their associated data. Installed into share/lua/example.")
SET(CPACK_COMPONENT_EXAMPLE_DISPLAY_NAME "lua Examples")
SET(CPACK_COMPONENT_HEADER_DESCRIPTION "Headers needed for development. Installed into include.")
SET(CPACK_COMPONENT_HEADER_DISPLAY_NAME "lua Development Headers")
SET(CPACK_COMPONENT_LIBRARY_DESCRIPTION "Static and import libraries needed for development. Installed into lib or bin.")
SET(CPACK_COMPONENT_LIBRARY_DISPLAY_NAME "lua Development Libraries")
SET(CPACK_COMPONENT_OTHER_DESCRIPTION "Other unspecified content. Installed into share/lua/etc.")
SET(CPACK_COMPONENT_OTHER_DISPLAY_NAME "lua Unspecified Content")
SET(CPACK_COMPONENT_RUNTIME_DESCRIPTION "Executables and runtime libraries. Installed into bin.")
SET(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "lua Runtime")
SET(CPACK_COMPONENT_TEST_DESCRIPTION "Tests and associated data. Installed into share/lua/test.")
SET(CPACK_COMPONENT_TEST_DISPLAY_NAME "lua Tests")
SET(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
SET(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
SET(CPACK_GENERATOR "ZIP")
SET(CPACK_INSTALL_CMAKE_PROJECTS "/home/tim/Projects/cpp/raid/cmake/configs/linux-deps;flexy;ALL;/")
SET(CPACK_INSTALL_PREFIX "/usr/local")
SET(CPACK_MODULE_PATH "/home/tim/Projects/cpp/raid/vendor/lua/cmake;/home/tim/Projects/cpp/raid/cmake/cmake-files/sugar/collecting;/home/tim/Projects/cpp/raid/cmake/cmake-files/sugar/core;/home/tim/Projects/cpp/raid/cmake/cmake-files/sugar/print;/home/tim/Projects/cpp/raid/cmake/cmake-files/sugar/utility")
SET(CPACK_NSIS_DISPLAY_NAME "lua 5.3.2")
SET(CPACK_NSIS_INSTALLER_ICON_CODE "")
SET(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
SET(CPACK_NSIS_PACKAGE_NAME "lua 5.3.2")
SET(CPACK_OUTPUT_CONFIG_FILE "/home/tim/Projects/cpp/raid/cmake/configs/linux-deps/CPackConfig.cmake")
SET(CPACK_PACKAGE_DEFAULT_LOCATION "/")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake-3.7/Templates/CPack.GenericDescription.txt")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "flexy built using CMake")
SET(CPACK_PACKAGE_FILE_NAME "lua-5.3.2-Linux")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "lua 5.3.2")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "lua 5.3.2")
SET(CPACK_PACKAGE_NAME "lua")
SET(CPACK_PACKAGE_RELOCATABLE "true")
SET(CPACK_PACKAGE_VENDOR "LuaDist")
SET(CPACK_PACKAGE_VERSION "5.3.2")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "1")
SET(CPACK_RESOURCE_FILE_LICENSE "/usr/share/cmake-3.7/Templates/CPack.GenericLicense.txt")
SET(CPACK_RESOURCE_FILE_README "/usr/share/cmake-3.7/Templates/CPack.GenericDescription.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "/usr/share/cmake-3.7/Templates/CPack.GenericWelcome.txt")
SET(CPACK_SET_DESTDIR "OFF")
SET(CPACK_SOURCE_7Z "")
SET(CPACK_SOURCE_CYGWIN "")
SET(CPACK_SOURCE_GENERATOR "TBZ2;TGZ;TXZ;TZ")
SET(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/home/tim/Projects/cpp/raid/cmake/configs/linux-deps/CPackSourceConfig.cmake")
SET(CPACK_SOURCE_RPM "OFF")
SET(CPACK_SOURCE_TBZ2 "ON")
SET(CPACK_SOURCE_TGZ "ON")
SET(CPACK_SOURCE_TXZ "ON")
SET(CPACK_SOURCE_TZ "ON")
SET(CPACK_SOURCE_ZIP "OFF")
SET(CPACK_STRIP_FILES "TRUE")
SET(CPACK_SYSTEM_NAME "Linux")
SET(CPACK_TOPLEVEL_TAG "Linux")
SET(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "/home/tim/Projects/cpp/raid/cmake/configs/linux-deps/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
