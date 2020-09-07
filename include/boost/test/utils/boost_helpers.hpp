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

#if !defined(BOOST_TEST_CONFIG_HPP_071894GER)
/// @todo make it MSVC as well
#warning "including without config"
#endif

#if !defined(BOOST_TEST_USE_BOOST)
    #if (__cplusplus < 201103L)
        #define BOOST_TEST_USE_BOOST 1
    #else
        #define BOOST_TEST_USE_BOOST 0
    #endif
#endif


// include boost/config if we are using boost
#if BOOST_TEST_USE_BOOST==1
  #include <boost/config.hpp>
#else
  #if (__cplusplus < 201103L)
    #error "Cannot use BOOST_TEST_USE_BOOST==0 for pre-C++11 compilers"
  #endif
#endif


// ************************************************************************** //
// **************               preprocessor caps              ************** //
// ************************************************************************** //
// This definition should be performed very early in the include steps as
// other boost libraries include the preprocessor library
#if BOOST_TEST_USE_BOOST==1 && !defined(BOOST_PP_VARIADICS) /* we can change this only if not already defined */

#ifdef __PGI
#define BOOST_PP_VARIADICS 1
#endif

#if BOOST_CLANG
#define BOOST_PP_VARIADICS 1
#endif

#if defined(BOOST_GCC) && (BOOST_GCC >= 4 * 10000 + 8 * 100)
#define BOOST_PP_VARIADICS 1
#endif

#if defined(__NVCC__)
#define BOOST_PP_VARIADICS 1
#endif

#endif /* BOOST_TEST_USE_BOOST==1 && !defined(BOOST_PP_VARIADICS) */


// ************************************************************************** //
// **************               macro indirections             ************** //
// ************************************************************************** //

#if BOOST_TEST_USE_BOOST==1
  #include <boost/detail/workaround.hpp>
#else
  #define BOOST_WORKAROUND(x, y) (x) != 0 && (x y)
  #define BOOST_TESTED_AT(x) != ((x) - (x))
#endif

// BOOST_TEST_ATTRIBUTE_UNUSED <-> BOOST_ATTRIBUTE_UNUSED
#if BOOST_TEST_USE_BOOST==1
  // from boost/config
  #define BOOST_TEST_ATTRIBUTE_UNUSED BOOST_ATTRIBUTE_UNUSED
#else
  // clang
  #define BOOST_TEST_ATTRIBUTE_UNUSED __attribute__((__unused__))
#endif

// BOOST_TEST_OVERRIDE <-> BOOST_OVERRIDE
#if BOOST_TEST_USE_BOOST==1
  // from boost/config
  #define BOOST_TEST_OVERRIDE BOOST_OVERRIDE
#else
  #define BOOST_TEST_OVERRIDE override
#endif

// BOOST_TEST_CURRENT_FUNCTION <-> BOOST_CURRENT_FUNCTION
#if BOOST_TEST_USE_BOOST==1
  #include <boost/current_function.hpp>
  #define BOOST_TEST_CURRENT_FUNCTION BOOST_CURRENT_FUNCTION
#else
  #define BOOST_TEST_CURRENT_FUNCTION __func__
#endif

// BOOST_TEST_VERSION <-> BOOST_VERSION
#if BOOST_TEST_USE_BOOST==1
  #include <boost/version.hpp>
  #define BOOST_TEST_VERSION BOOST_VERSION
#else
  #define BOOST_TEST_VERSION 107401
#endif

// BOOST_TEST_DEDUCED_TYPENAME <-> BOOST_DEDUCED_TYPENAME
#if BOOST_TEST_USE_BOOST==1
  #define BOOST_TEST_DEDUCED_TYPENAME BOOST_DEDUCED_TYPENAME
  #define BOOST_TEST_CTOR_TYPENAME BOOST_CTOR_TYPENAME
#else
  #define BOOST_TEST_DEDUCED_TYPENAME typename
  #define BOOST_TEST_CTOR_TYPENAME typename
#endif


#if BOOST_TEST_USE_BOOST==0
#  define BOOST_SYMBOL_EXPORT __attribute__((__visibility__("default")))
#  define BOOST_SYMBOL_VISIBLE __attribute__((__visibility__("default")))
#endif



// ************************************************************************** //
// **************               function indirections          ************** //
// ************************************************************************** //

// BOOST_TEST_FUNCTION <-> std::function/boost::function
#if BOOST_TEST_USE_BOOST==1
  #include <boost/function/function0.hpp>
  #include <boost/function/function1.hpp>
  #include <boost/function/function2.hpp>
  #define BOOST_TEST_FUNCTION boost::function
#else
  #include <functional>
  #define BOOST_TEST_FUNCTION std::function
#endif


// enable if
#if BOOST_TEST_USE_BOOST==1
  #include <boost/utility/enable_if.hpp>
  #define BOOST_TEST_ENABLE_IF boost::enable_if_c
#else
  #include <type_traits>
  #define BOOST_TEST_ENABLE_IF std::enable_if
#endif

// boost::reference_wrapper
#if BOOST_TEST_USE_BOOST==1
  #include <boost/core/ref.hpp>
  #define BOOST_TEST_REFERENCE_WRAPPER boost::reference_wrapper
  #define BOOST_TEST_REF boost::ref
#else
  #include <functional>
  #define BOOST_TEST_REFERENCE_WRAPPER std::reference_wrapper
  #define BOOST_TEST_REF std::ref
#endif

// make_shared
#if BOOST_TEST_USE_BOOST==1
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
#if BOOST_TEST_USE_BOOST==1
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
# define BOOST_TEST_ASSERT_MSG(expr, msg) assert((expr)&&(msg))


#define BOOST_TEST_PLATFORM BOOST_PLATFORM

// static_assert with or without message
// addressof
// bind

// BOOST_NO_EXCEPTIONS


// BOOST_NORETURN
#if !defined(BOOST_NORETURN)
#  if defined(__has_cpp_attribute)
#    if __has_cpp_attribute(noreturn)
#      define BOOST_NORETURN [[noreturn]]
#    endif
#  elif defined(_MSC_VER)
#    define BOOST_NORETURN __declspec(noreturn)
#  elif defined(__GNUC__) || defined(__CODEGEARC__) && defined(__clang__)
#    define BOOST_NORETURN __attribute__ ((__noreturn__))
#  endif
#endif

// BOOST_COMPILER
// BOOST_STDLIB
// BOOST_FALLTHROUGH
// BOOST_NO_FENV_H

// BOOST_DELETED_FUNCTION
// BOOST_DEFAULTED_FUNCTION

// boost::conditional

#define BOOST_TEST_STRINGIZE(X) BOOST_TEST_DO_STRINGIZE_PRIVATE(X)
#define BOOST_TEST_DO_STRINGIZE_PRIVATE(X) #X

//
#define BOOST_TEST_JOIN(X, Y) BOOST_TEST_DO_JOIN_PRIVATE(X, Y)
#define BOOST_TEST_DO_JOIN_PRIVATE(X, Y) BOOST_TEST_DO_JOIN_PRIVATE2(X,Y)
#define BOOST_TEST_DO_JOIN_PRIVATE2(X, Y) X##Y




#endif /* BOOST_TEST_BOOST_HELPERS_HPP__ */
