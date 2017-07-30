# boost-decl.cmake
# declares the components of boost.test and their dependencies

# The following variables should be defined:
#
# * ``BOOST_LIB_<current_package>_COMPONENTS``: list indicating all the components of the current package
# * ``BOOST_LIB_<current_package>_COMPONENTS_<component>_DEPENDENCY`` list indicating all the dependencies of the ``component``

set(_current_package "TEST")
set(BOOST_LIB_${_current_package}_COMPONENTS "build" "doc" "test")

# checking no aliasing
foreach(_component IN LISTS BOOST_LIB_${_current_package}_COMPONENTS)
  string(TOUPPER ${_component} _current_component_upper)
  if(DEFINED BOOST_LIB_${_current_package}_COMPONENTS_${_current_component_upper}_DEPENDENCY)
    message(FATAL_ERROR "The variable 'BOOST_LIB_${_current_package}_COMPONENTS_${_current_component_upper}_DEPENDENCY' is already defined")
  endif()
endforeach()

set(BOOST_LIB_${_current_package}_COMPONENTS_BUILD_DEPENDENCY
  "system:build"
  "core:build"
  "config:build"
  "predef:build"
  "assert:build"
  "function:build"
  "mpl:build"
  "throw_exception:build"
  "static_assert:build"
  "preprocessor:build"
  "functional:build"
  "utility:build"
  "smart_ptr:build"
  "bind:build"
  "exception:build"
  "iterator:build"
  "move:build"
  "timer:build"
  "io:build"
  "algorithm:build"
  "range:build"
)
set(BOOST_LIB_${_current_package}_COMPONENTS_DOC_DEPENDENCY ) #"quickbook:core")
set(BOOST_LIB_${_current_package}_COMPONENTS_TEST_DEPENDENCY "test:build")
