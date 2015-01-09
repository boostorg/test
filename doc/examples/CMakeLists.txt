cmake_minimum_required(VERSION 2.8.11)
project(examples)
enable_testing()

# not meant to be included in the documentation
set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/../../../..)
message(STATUS "$BOOST_ROOT = ${BOOST_ROOT}")

set(EXAMPLES_SRC
    example64.cpp
    example68.cpp
    )

foreach(example_file IN LISTS EXAMPLES_SRC)

  get_filename_component(example_project ${example_file} NAME_WE)

  # creates the executable
  add_executable(test_${example_project} ${example_file})
  # indicates the include paths
  target_include_directories(test_${example_project} PRIVATE ${BOOST_ROOT})

  # declares a test with our executable
  add_test(NAME test-${example_project} COMMAND test_${example_project})


endforeach(example_file)

