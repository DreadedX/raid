# CMake generated Testfile for 
# Source directory: /home/tim/Projects/cpp/raid/vendor/shaderc/third_party/spirv-tools
# Build directory: /home/tim/Projects/cpp/raid/cmake/configs/linux/vendor/shaderc/third_party/spirv-tools
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(spirv-tools-copyrights "/usr/sbin/python3" "utils/check_copyright.py")
set_tests_properties(spirv-tools-copyrights PROPERTIES  WORKING_DIRECTORY "/home/tim/Projects/cpp/raid/vendor/shaderc/third_party/spirv-tools")
subdirs("external")
subdirs("source")
subdirs("tools")
subdirs("test")
subdirs("examples")
