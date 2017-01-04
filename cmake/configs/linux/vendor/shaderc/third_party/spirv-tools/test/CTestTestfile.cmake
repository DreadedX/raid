# CMake generated Testfile for 
# Source directory: /home/tim/Projects/cpp/raid/vendor/shaderc/third_party/spirv-tools/test
# Build directory: /home/tim/Projects/cpp/raid/cmake/configs/linux/vendor/shaderc/third_party/spirv-tools/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(spirv-tools-test_spirv_unit_tests "/home/tim/Projects/cpp/raid/build/linux-debug/bin/test_spirv_unit_tests")
add_test(spirv-tools-test_diagnostic "/home/tim/Projects/cpp/raid/build/linux-debug/bin/test_diagnostic")
add_test(spirv-tools-test_c_interface "/home/tim/Projects/cpp/raid/build/linux-debug/bin/test_c_interface")
add_test(spirv-tools-test_cpp_interface "/home/tim/Projects/cpp/raid/build/linux-debug/bin/test_cpp_interface")
add_test(spirv-tools-test_parse_number "/home/tim/Projects/cpp/raid/build/linux-debug/bin/test_parse_number")
add_test(spirv-tools-test_log "/home/tim/Projects/cpp/raid/build/linux-debug/bin/test_log")
subdirs("opt")
subdirs("val")
