//  (C) Copyright Gennadiy Rozental 2001-2003.
//  See accompanying license for terms and conditions of use.

//  See http://www.boost.org for most recent version including documentation.
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

// STL
#include <iterator>     // for std::distance
#include <cstddef>      // for std::ptrdiff_t

namespace boost {

namespace unit_test_framework {

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

} // namespace detail

} // namespace unit_test_framework

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.14  2003/10/27 07:13:13  rogeeff
//  licence update
//
//  Revision 1.13  2003/06/09 08:39:28  rogeeff
//  1.30.beta1
//

// ***************************************************************************

#endif // BOOST_UNIT_TEST_CONFIG_HPP
