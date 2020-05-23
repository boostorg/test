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

#include <boost/test/detail/config.hpp>

#if (__cplusplus < 201103L)
  #define BOOST_TEST_USE_BOOST
#endif

#undef BOOST_NO_CXX11_HDR_FUNCTIONAL


#if defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)
#include <boost/function/function0.hpp>
#else
#include <functional>
#endif


// std::function abstract
#if defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)
#define BOOST_TEST_FUNCTION boost::function
#else
#define BOOST_TEST_FUNCTION std::function
#endif

#if !defined(BOOST_TEST_USE_BOOST)
#define BOOST_OVERRIDE override
#endif



// enable if
#if defined(BOOST_TEST_USE_BOOST)
#include <boost/utility/enable_if.hpp>
#define BOOST_TEST_ENABLE_IF boost::enable_if_c
#else
#include <type_traits>
#define BOOST_TEST_ENABLE_IF std::enable_if
#endif


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

#if !defined(BOOST_TEST_USE_BOOST)
#define BOOST_DEDUCED_TYPENAME typename
#define BOOST_CTOR_TYPENAME typename
#endif

# include <cassert>
# define BOOST_TEST_ASSERT(expr) assert(expr)


// boost::ref
// static_assert with or without message
// addressof
// shared_ptr
// bind


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



// current function
#if defined(BOOST_TEST_USE_BOOST)
  #include <boost/current_function.hpp>
#else
  #if defined(BOOST_CURRENT_FUNCTION)
    #error BOOST_CURRENT_FUNCTION already defined
  #endif
  #define BOOST_CURRENT_FUNCTION __func__
#endif

#endif /* BOOST_TEST_BOOST_HELPERS_HPP__ */
