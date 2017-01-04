# CMake generated Testfile for 
# Source directory: /home/tim/Projects/cpp/raid/vendor/shaderc/third_party
# Build directory: /home/tim/Projects/cpp/raid/cmake/configs/linux/vendor/shaderc/third_party
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(glslang-testsuite "bash" "runtests")
set_tests_properties(glslang-testsuite PROPERTIES  WORKING_DIRECTORY "/home/tim/Projects/cpp/raid/cmake/configs/linux/vendor/shaderc/third_party/test-glslang/")
subdirs("googletest")
subdirs("glslang")
subdirs("spirv-tools")
