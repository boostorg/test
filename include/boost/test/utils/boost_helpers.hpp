//  (C) Copyright Raffi Enficiaud 2019.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! Defines C++11 support indirections
// ***************************************************************************

#ifndef BOOST_TEST_BOOST_HELPERS_HPP__
#define BOOST_TEST_BOOST_HELPERS_HPP__

#if (__cplusplus < 201103L)
  #define BOOST_TEST_USE_BOOST
  #include <boost/config.hpp>
#else
  #if defined(BOOST_TEST_USE_BOOST)
    #error "BOOST_TEST_USE_BOOST already defined"
  #endif
#endif

// ************************************************************************** //
// **************               macro indirections             ************** //
// ************************************************************************** //

// BOOST_TEST_ATTRIBUTE_UNUSED <-> BOOST_ATTRIBUTE_UNUSED
#if defined(BOOST_TEST_USE_BOOST)
  // from boost/config
  #define BOOST_TEST_ATTRIBUTE_UNUSED BOOST_ATTRIBUTE_UNUSED
#else
  // clang
  #define BOOST_TEST_ATTRIBUTE_UNUSED __attribute__((__unused__))
#endif

// BOOST_TEST_OVERRIDE <-> BOOST_OVERRIDE
#if defined(BOOST_TEST_USE_BOOST)
  // from boost/config
  #define BOOST_TEST_OVERRIDE BOOST_OVERRIDE
#else
  #define BOOST_TEST_OVERRIDE override
#endif

// BOOST_TEST_CURRENT_FUNCTION <-> BOOST_CURRENT_FUNCTION
#if defined(BOOST_TEST_USE_BOOST)
  #include <boost/current_function.hpp>
  #define BOOST_TEST_CURRENT_FUNCTION BOOST_CURRENT_FUNCTION
#else
  #define BOOST_TEST_CURRENT_FUNCTION __func__
#endif

// BOOST_TEST_VERSION <-> BOOST_VERSION
#if defined(BOOST_TEST_USE_BOOST)
  #include <boost/version.hpp>
  #define BOOST_TEST_VERSION BOOST_VERSION
#else
  #define BOOST_TEST_VERSION 107400
#endif

// BOOST_TEST_DEDUCED_TYPENAME <-> BOOST_DEDUCED_TYPENAME
#if defined(BOOST_TEST_USE_BOOST)
  #define BOOST_TEST_DEDUCED_TYPENAME BOOST_DEDUCED_TYPENAME
  #define BOOST_TEST_CTOR_TYPENAME BOOST_CTOR_TYPENAME
#else
  #define BOOST_TEST_DEDUCED_TYPENAME typename
  #define BOOST_TEST_CTOR_TYPENAME typename
#endif




// ************************************************************************** //
// **************               function indirections          ************** //
// ************************************************************************** //

// BOOST_TEST_FUNCTION <-> std::function/boost::function
#if defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)
  #include <boost/function/function0.hpp>
  #include <boost/function/function1.hpp>
  #include <boost/function/function2.hpp>
  #define BOOST_TEST_FUNCTION boost::function
#else
  #include <functional>
  #define BOOST_TEST_FUNCTION std::function
#endif


// enable if
#if defined(BOOST_TEST_USE_BOOST)
  #include <boost/utility/enable_if.hpp>
  #define BOOST_TEST_ENABLE_IF boost::enable_if_c
#else
  #include <type_traits>
  #define BOOST_TEST_ENABLE_IF std::enable_if
#endif

// boost::reference_wrapper
#if defined(BOOST_TEST_USE_BOOST)
  #include <boost/core/ref.hpp>
  #define BOOST_TEST_REFERENCE_WRAPPER boost::reference_wrapper
  #define BOOST_TEST_REF boost::ref
#else
  #include <functional>
  #define BOOST_TEST_REFERENCE_WRAPPER std::reference_wrapper
  #define BOOST_TEST_REF std::ref
#endif

// make_shared
#if defined(BOOST_TEST_USE_BOOST)
  #include <boost/make_shared.hpp>

  #define BOOST_TEST_SHARE_PTR boost::shared_ptr
  #define BOOST_TEST_MAKE_SHARED boost::make_shared
#else
  #include <memory>
  #define BOOST_TEST_SHARE_PTR std::shared_ptr
  #define BOOST_TEST_MAKE_SHARED std::make_shared
#endif



// ************************************************************************** //
// **************               type indirections              ************** //
// ************************************************************************** //

// true_type
#if defined(BOOST_TEST_USE_BOOST)
  #include <boost/mpl/bool.hpp>
  namespace boost { namespace unit_test {
    typedef mpl::true_  bt_true_type;
    typedef mpl::false_ bt_false_type;
  }}
#else
  #include <type_traits>
  namespace boost { namespace unit_test {
    using bt_true_type = std::true_type;
    using bt_false_type = std::false_type;
  }}
#endif

namespace boost { namespace unit_test {
template <bool b>
struct bt_bool : bt_true_type {
};
template <>
struct bt_bool<false> : bt_false_type {
};
}}


# include <cassert>
# define BOOST_TEST_ASSERT(expr) assert(expr)


// static_assert with or without message
// addressof
// bind

// BOOST_NO_EXCEPTIONS
// BOOST_NORETURN

// BOOST_PLATFORM
// BOOST_COMPILER
// BOOST_STDLIB
// BOOST_FALLTHROUGH
// BOOST_NO_FENV_H
// BOOST_JOIN



#endif /* BOOST_TEST_BOOST_HELPERS_HPP__ */
