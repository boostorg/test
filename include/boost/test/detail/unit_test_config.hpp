//  (C) Copyright Gennadiy Rozental 2001-2004.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : contains workarounds and works as a central place for configurable types
// ***************************************************************************

#ifndef BOOST_UNIT_TEST_CONFIG_HPP
#define BOOST_UNIT_TEST_CONFIG_HPP

// BOOST
#include <boost/config.hpp> // compilers workarounds and std::ptrdiff_t

// Boost.Test
#include <boost/test/detail/basic_cstring/basic_cstring.hpp>
#include <boost/test/detail/basic_cstring/io.hpp>
#define BOOST_TEST_STRING_LITERAL( s ) boost::unit_test::literal_string( s, sizeof( s ) - 1 )
#define BOOST_TEST_EMPTY_STRING BOOST_TEST_STRING_LITERAL( "" )

// STL
#include <iterator>     // for std::distance
#include <cstddef>      // for std::ptrdiff_t

namespace boost {

namespace unit_test {

typedef unsigned long   unit_test_counter;
typedef char const*     c_string_literal;

namespace detail {

#ifdef BOOST_NO_STD_DISTANCE
template <class T>
std::ptrdiff_t distance( T const& x_, T const& y_ )
{ 
    std::ptrdiff_t res = 0;

    std::distance( x_, y_, res );

    return res;
}
#else
using std::distance;
#endif

#define BOOST_TEST_L( s ) const_string( s, sizeof(s) )

} // namespace detail

} // namespace unit_test

namespace unit_test_framework = unit_test;

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.18  2004/05/21 06:19:35  rogeeff
//  licence update
//
//  Revision 1.17  2004/05/11 11:00:53  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//
//  Revision 1.16  2003/12/01 00:41:56  rogeeff
//  prerelease cleaning
//
// ***************************************************************************

#endif // BOOST_UNIT_TEST_CONFIG_HPP
