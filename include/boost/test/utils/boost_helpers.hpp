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

#ifndef BOOST_OVERRIDE
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


#endif /* BOOST_TEST_BOOST_HELPERS_HPP__ */
