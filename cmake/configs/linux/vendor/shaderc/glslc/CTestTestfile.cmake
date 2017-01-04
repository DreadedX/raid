# CMake generated Testfile for 
# Source directory: /home/tim/Projects/cpp/raid/vendor/shaderc/glslc
# Build directory: /home/tim/Projects/cpp/raid/cmake/configs/linux/vendor/shaderc/glslc
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(glslc_file "/home/tim/Projects/cpp/raid/build/linux-debug/bin/glslc_file_test")
add_test(glslc_resource_parse "/home/tim/Projects/cpp/raid/build/linux-debug/bin/glslc_resource_parse_test")
add_test(glslc_stage "/home/tim/Projects/cpp/raid/build/linux-debug/bin/glslc_stage_test")
subdirs("test")
