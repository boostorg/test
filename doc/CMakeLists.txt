#
# Copyright (c) 2003 Boost.Test contributors
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Standalone project whose only purpose is to give MrDocs a compilation
# database for mrdocs.cpp, the umbrella translation unit that includes the
# public headers. Nothing is built: MrDocs only configures this project and
# reads compile_commands.json.
#
# The library's own CMakeLists.txt cannot serve this purpose because it is a
# modular-Boost library that has to be added from the superproject. So this
# project adds the superproject instead, restricted to Boost.Test and its
# dependencies, and picks up the include paths from the Boost::unit_test_framework
# target's interface.

cmake_minimum_required(VERSION 3.8...3.22)

project(boost_test_mrdocs LANGUAGES CXX)

# MrDocs forces CMAKE_EXPORT_COMPILE_COMMANDS=ON, which would dump every target
# of the superproject into the compilation database. Turn it off globally and
# switch it on for our target alone.
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

if(NOT DEFINED ENV{BOOST_SRC_DIR})
  message(FATAL_ERROR
    "BOOST_SRC_DIR is not set. It normally comes from the cpp-reference "
    "extension's `dependencies` block in the Antora playbook; build_antora.sh "
    "also derives it from a surrounding Boost superproject checkout.")
endif()

# Configure only what Boost.Test needs; the superproject resolves the
# dependencies listed in ../CMakeLists.txt for us.
set(BOOST_INCLUDE_LIBRARIES test)
add_subdirectory($ENV{BOOST_SRC_DIR} deps/boost EXCLUDE_FROM_ALL)

# An object library, not an executable: unit_test.hpp brings in a main()
# that wants init_unit_test_suite(), and there is nothing here to link.
add_library(mrdocs OBJECT mrdocs.cpp)

# This worktree's headers must win over whatever the Boost checkout carries,
# which matters when BOOST_SRC_DIR is a clone rather than the superproject this
# library sits in.
get_filename_component(BOOST_TEST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include" ABSOLUTE)
target_include_directories(mrdocs BEFORE PRIVATE ${BOOST_TEST_INCLUDE_DIR})
target_link_libraries(mrdocs PRIVATE Boost::unit_test_framework)
set_target_properties(mrdocs PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  EXPORT_COMPILE_COMMANDS ON)
