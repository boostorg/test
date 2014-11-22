cmake_minimum_required(VERSION 2.8.11)
project(my_first_test)
enable_testing()

# looking for the boost installation path
find_package(Boost COMPONENTS unit_test_framework)

# creates the executable
add_executable(test_executable test_file.cpp)
target_link_libraries(test_executable ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

# declares a test with our executable
add_test(NAME test1 COMMAND test_executable)


