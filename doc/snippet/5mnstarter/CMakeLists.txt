cmake_minimum_required(VERSION 2.8.11)
project(my_first_test)
enable_testing()

# Add any relevant version here
set(Boost_ADDITIONAL_VERSIONS "1.58" "1.58.0")

find_package(Boost 1.58 REQUIRED COMPONENTS unit_test_framework)

# creates the executable
add_executable(test_executable test_file.cpp)
# indicates the include paths
target_include_directories(test_executable PRIVATE ${Boost_INCLUDE_DIRS})
# indicates the link paths
target_link_libraries(test_executable ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

# declares a test with our executable
add_test(NAME test1 COMMAND test_executable)


