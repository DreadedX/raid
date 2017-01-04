# CMake generated Testfile for 
# Source directory: /home/tim/Projects/cpp/raid/vendor/shaderc/glslc/test
# Build directory: /home/tim/Projects/cpp/raid/cmake/configs/linux/vendor/shaderc/glslc/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(glslc_tests "/usr/sbin/python" "/home/tim/Projects/cpp/raid/vendor/shaderc/glslc/test/glslc_test_framework.py" "/home/tim/Projects/cpp/raid/build/linux-debug/bin/glslc" "/home/tim/Projects/cpp/raid/build/linux-debug/bin/spirv-dis" "--test-dir" "/home/tim/Projects/cpp/raid/vendor/shaderc/glslc/test")
