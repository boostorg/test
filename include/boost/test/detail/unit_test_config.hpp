//  (C) Copyright Gennadiy Rozental 2001-2002.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Id$
//
//  Description :
// ***************************************************************************

#ifndef BOOST_UNIT_TEST_CONFIG_HPP
#define BOOST_UNIT_TEST_CONFIG_HPP

// BOOST
#include <boost/config.hpp>

// STL
#include <iterator>

namespace boost {

namespace unit_test_framework {

typedef unsigned long unit_test_counter;

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
//  Revision 1.8  2002/08/20 22:24:53  rogeeff
//  all formal arguments trailed with underscore
//
//  Revision 1.7  2002/08/20 08:52:41  rogeeff
//  cvs keywords added
//
//   8 Aug 02  Parameters definition separated (Gennadiy Rozental)
//   5 Oct 01  Initial version (Gennadiy Rozental)

// ***************************************************************************

#endif // BOOST_UNIT_TEST_CONFIG_HPP
