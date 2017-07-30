# Copyright 2017, Raffi Enficiaud

# Use, modification, and distribution are subject to the
# Boost Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# See http://www.boost.org/libs/test for the library home page.

# This cmake file defines the tests of the boost.test library
# It is not meant to be included independently of the boost superproject cmake.

####
# Documentation files (files only, no target)
file(GLOB_RECURSE
     BOOST_TEST_DOC_FILES
     ${CMAKE_CURRENT_LIST_DIR}/*.qbk)
add_custom_target(
  quickbook
  SOURCES ${BOOST_TEST_DOC_FILES})
set_property(TARGET quickbook PROPERTY FOLDER "boost.${BOOST_CURRENT_PACKAGE}/${BOOST_CURRENT_COMPONENT}")
