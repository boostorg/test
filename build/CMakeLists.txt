# Copyright 2014, Raffi Enficiaud

# Use, modification, and distribution are subject to the
# Boost Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# See http://www.boost.org/libs/test for the library home page.

cmake_minimum_required(VERSION 3.2)
project(boost.test)


if(FALSE)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set(CMAKE_MACOSX_RPATH ON)

  # build type, by default to release (with optimisations)
  if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
  endif()

  if(NOT WITHOUT_TESTS)
    # ctest sets BUILD_TESTING automatically, but does not seem to serve its purpose.
    enable_testing()
  endif()


  include(CheckCXXCompilerFlag)
  include(CheckIncludeFileCXX)

  if(NOT MSVC)
    # c++11 options
    check_cxx_compiler_flag(-std=c++11 HAS_CXX11_FLAG)
    check_cxx_compiler_flag(-std=c++0x HAS_CXX0X_FLAG)
    if(HAS_CXX11_FLAG)
      message(STATUS "Compiling with C++11 support")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif(HAS_CXX0X_FLAG)
      #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    endif()
  endif()

  if(MSVC)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)
    set(MSVC_Additional_flags "/fp:fast /GF /Oy /GT /Ox /Ob2 /Oi /Os")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${MSVC_Additional_flags}")
  endif()

endif()


add_definitions(-DBOOST_TEST_NO_LIB)

# global path
set(BOOST_TEST_ROOT_DIR ${BoostTest_SOURCE_DIR}/..)
set(BOOST_ROOT_DIR ${BOOST_TEST_ROOT_DIR}/../..)
get_filename_component(BOOST_TEST_ROOT_DIR_ABS ${BoostTest_SOURCE_DIR}/.. ABSOLUTE)
get_filename_component(BOOST_ROOT_DIR_ABS ${BOOST_ROOT_DIR} ABSOLUTE)


# include globs
file(GLOB_RECURSE
     BOOST_UTF_HEADERS
     ../include/*.hpp
     ../include/*.ipp)

# organize files
if(FALSE)
foreach(_h IN LISTS BOOST_UTF_HEADERS)
  get_filename_component(_hh ${_h} ABSOLUTE)
  file(RELATIVE_PATH _v ${BOOST_TEST_ROOT_DIR_ABS}/include/boost/test ${_hh})
  get_filename_component(_v "${_v}" DIRECTORY)
  string(REPLACE "/" "\\" _v "${_v}")
  source_group(${_v} FILES ${_h})
endforeach()
endif()

set(BOOST_UTF_SRC
    ../src/compiler_log_formatter.cpp
    ../src/debug.cpp
    ../src/decorator.cpp
    ../src/execution_monitor.cpp
    ../src/framework.cpp
    ../src/junit_log_formatter.cpp
    ../src/plain_report_formatter.cpp
    ../src/progress_monitor.cpp
    ../src/results_collector.cpp
    ../src/results_reporter.cpp
    ../src/test_framework_init_observer.cpp
    ../src/test_tools.cpp
    ../src/test_tree.cpp
    ../src/unit_test_log.cpp
    ../src/unit_test_main.cpp
    ../src/unit_test_monitor.cpp
    ../src/unit_test_parameters.cpp
    ../src/xml_log_formatter.cpp
    ../src/xml_report_formatter.cpp
    )

# defining the main target, type depends from the main super project
add_library(boost_test
            ${BOOST_UTF_HEADERS}
            ${BOOST_UTF_SRC})
target_compile_definitions(
  boost_test
  PUBLIC
    BOOST_TEST_NO_AUTO_LINK=1
    $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:BOOST_TEST_DYN_LINK=1>
  )
target_include_directories(
  boost_test
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../include>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(boost_test
  PUBLIC
    boost::config
    boost::system
    boost::type_traits
    boost::mpl
    boost::function
    boost::throw_exception # this one is indirect from boost::function
    boost::integer # this one is indirect from boost::function
    boost::static_assert # this one is indirect from boost::integer, but also here
    boost::type_index # this one is indirect from boost::function
    boost::preprocessor
    boost::functional # indirect from type_index
    boost::detail # indirect from functional
    boost::utility
    boost::smart_ptr
    boost::bind # indirect from boost::function
    boost::exception
    boost::iterator
    boost::move # indirect from boost::smart_ptr
    boost::timer
    boost::io
    boost::algorithm
    boost::range # indirect from boost::algorithm
  )

# defining the header only target
add_library(boost_test_header_only INTERFACE)
target_include_directories(boost_test_header_only
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../include>
    $<INSTALL_INTERFACE:include>)
target_compile_definitions(
  boost_test_header_only
  INTERFACE
    BOOST_TEST_NO_AUTO_LINK=1
    $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:BOOST_TEST_DYN_LINK=1>
)

install(
  TARGETS boost_test
  RUNTIME DESTINATION lib
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

if(DEFINED BOOST_CURRENT_PACKAGE)
  add_library(boost::${BOOST_CURRENT_PACKAGE} ALIAS boost_test)
  add_library(boost::${BOOST_CURRENT_PACKAGE}::header ALIAS boost_test_header_only)
  set_target_properties(boost_test PROPERTIES FOLDER "boost.${BOOST_CURRENT_PACKAGE}/${BOOST_CURRENT_COMPONENT}")
endif()
