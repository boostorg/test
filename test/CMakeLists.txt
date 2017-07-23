# Copyright 2017, Raffi Enficiaud

# Use, modification, and distribution are subject to the
# Boost Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# See http://www.boost.org/libs/test for the library home page.

# This cmake file defines the tests of the boost.test library
# It is not meant to be included independently of the boost superproject cmake.

project(boost:test:test)

if(NOT (TARGET boost_test_framework))
  message(FATAL_ERROR "This cmake file can only be included after the test:build component")
endif()

#message(AUTHOR_WARNING "BOOST_CURRENT_PACKAGE: ${BOOST_CURRENT_PACKAGE}")
#message(AUTHOR_WARNING "BOOST_CURRENT_COMPONENT: ${BOOST_CURRENT_COMPONENT}")

# unit tests folder
set(BOOST_TEST_UNITTESTS_FOLDER ${CMAKE_CURRENT_LIST_DIR})

# documentation tests
file(GLOB_RECURSE
     BOOST_UTF_DOC_EXAMPLES
     ${BOOST_TEST_ROOT_DIR}/doc/examples/*.cpp)

foreach(_h IN LISTS BOOST_UTF_DOC_EXAMPLES)
  get_filename_component(_hh ${_h} NAME_WE)
  add_executable(doc-${_hh} ${_h} ${BOOST_TEST_ROOT_DIR}/doc/examples/${_hh}.output)
  target_include_directories(doc-${_hh} PUBLIC ${BOOST_TEST_ROOT_DIR}/include/)
  set_target_properties(doc-${_hh} PROPERTIES FOLDER "boost.${BOOST_CURRENT_PACKAGE}/${BOOST_CURRENT_COMPONENT}/Doc examples")

  add_test(NAME doc-${_hh}-test
           COMMAND doc-${_hh})
  get_filename_component(_ext ${_h} EXT)
  string(FIND ${_ext} "fail" _index_fail)
  if(${_index_fail} GREATER -1)
    #message(STATUS "test ${_hh}.${_ext} = ${_index_fail}")
    set_tests_properties(doc-${_hh}-test PROPERTIES WILL_FAIL TRUE)
  endif()
endforeach()



# datasets
file(GLOB
     BOOST_TEST_UNITTESTS_DATASET
     ${BOOST_TEST_UNITTESTS_FOLDER}/test-organization-ts/datasets-test/*.cpp
     ${BOOST_TEST_UNITTESTS_FOLDER}/test-organization-ts/datasets-test/*.hpp)
add_executable(boost_test_datasets ${BOOST_TEST_UNITTESTS_DATASET})
target_include_directories(boost_test_datasets PUBLIC ${BOOST_TEST_ROOT_DIR}/include/)
target_link_libraries(boost_test_datasets boost_test_framework)
target_compile_definitions(boost_test_datasets PUBLIC "BOOST_TEST_DYN_LINK=1")
set_target_properties(boost_test_datasets PROPERTIES FOLDER "boost.${BOOST_CURRENT_PACKAGE}/${BOOST_CURRENT_COMPONENT}/Unit tests")
add_test(NAME bt-unittest-dataset
         COMMAND boost_test_datasets)


####
# TS writing-test-ts

set(BOOST_UTF_TESTS_IND_FILES
    writing-test-ts
    execution_monitor-ts
    framework-ts
    usage-variants-ts
    utils-ts
    test-organization-ts
    smoke-ts
  )

foreach(_ts IN LISTS BOOST_UTF_TESTS_IND_FILES)

  #message("parsing test suite ${_ts}")
  file(GLOB
       _boost_utf_current_tsuite
       ${BOOST_TEST_UNITTESTS_FOLDER}/${_ts}/*.cpp)


  foreach(_h IN LISTS _boost_utf_current_tsuite)
    get_filename_component(_hh ${_h} ABSOLUTE)
    get_filename_component(_name ${_h} NAME_WE)
    file(RELATIVE_PATH _v ${BOOST_TEST_UNITTESTS_FOLDER} ${_hh})
    #get_filename_component(_v "${_v}" DIRECTORY)
    #message("adding ${_ts}/${_name}")
    add_executable(${_name} ${_hh})
    target_link_libraries(${_name} PRIVATE boost_test_framework)
    set_target_properties(${_name} PROPERTIES FOLDER "boost.${BOOST_CURRENT_PACKAGE}/${BOOST_CURRENT_COMPONENT}/Unit tests/${_ts}")
    add_test(NAME bt-unittest-${_name}
             COMMAND ${_name})
  endforeach()

  unset(_boost_utf_current_tsuite)

endforeach() # test suite
