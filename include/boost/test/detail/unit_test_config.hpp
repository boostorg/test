//  (C) Copyright Gennadiy Rozental 2001-2005.
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

#ifndef BOOST_UNIT_TEST_CONFIG_HPP_071894GER
#define BOOST_UNIT_TEST_CONFIG_HPP_071894GER

// Boost
#include <boost/config.hpp> // compilers workarounds and std::ptrdiff_t
#include <boost/detail/workaround.hpp>

#if BOOST_WORKAROUND(__GNUC__, < 3) && !defined(__SGI_STL_PORT) && !defined(_STLPORT_VERSION)
#define BOOST_CLASSIC_IOSTREAMS
#else
#define BOOST_STANDARD_IOSTREAMS
#endif

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x570)) || \
    BOOST_WORKAROUND(__IBMCPP__, BOOST_TESTED_AT(600))     || \
    (defined __sgi && BOOST_WORKAROUND(_COMPILER_VERSION, BOOST_TESTED_AT(730)))
#define BOOST_TEST_SHIFTED_LINE
#endif

#if defined(BOOST_MSVC) || (defined(__BORLANDC__) && !defined(BOOST_DISABLE_WIN32))
#   define BOOST_TEST_CALL_DECL __cdecl
#else
#   define BOOST_TEST_CALL_DECL /**/
#endif

// Boost.Test
#include <boost/test/utils/basic_cstring/basic_cstring.hpp>
#include <boost/test/utils/basic_cstring/io.hpp>
#define BOOST_TEST_STRING_LITERAL( s ) boost::unit_test::literal_string( s, sizeof( s ) - 1 )
#define BOOST_TEST_STRINGIZE( s ) BOOST_TEST_STRING_LITERAL( BOOST_STRINGIZE( s ) )
#define BOOST_TEST_EMPTY_STRING BOOST_TEST_STRING_LITERAL( "" )

#if defined(BOOST_HAS_SIGACTION)
#define BOOST_TEST_SUPPORT_TIMEOUT
#endif

#if BOOST_WORKAROUND(__BORLANDC__, <= 0x570)           || \
    BOOST_WORKAROUND( __COMO__, <= 0x433 )             || \
    BOOST_WORKAROUND( __INTEL_COMPILER, <= 800 )       || \
    BOOST_WORKAROUND(__GNUC__, < 3)                    || \
    defined(__sgi) && _COMPILER_VERSION <= 730         || \
    BOOST_WORKAROUND(__IBMCPP__, BOOST_TESTED_AT(600)) || \
    defined(__DECCXX)
#define BOOST_TEST_NO_PROTECTED_USING
#endif

// STL
#include <iterator>     // for std::distance
#include <cstddef>      // for std::ptrdiff_t

namespace boost {

namespace unit_test {

typedef unsigned long   counter_t;

namespace ut_detail {

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

} // namespace ut_detail

} // namespace unit_test

namespace unit_test_framework = unit_test;

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.28  2005/02/01 06:40:07  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
//  Revision 1.27  2005/01/31 07:50:06  rogeeff
//  cdecl portability fix
//
//  Revision 1.26  2005/01/30 01:48:24  rogeeff
//  BOOST_TEST_STRINGIZE introduced
//  counter type renamed
//
//  Revision 1.25  2005/01/22 19:22:12  rogeeff
//  implementation moved into headers section to eliminate dependency of included/minimal component on src directory
//
//  Revision 1.24  2005/01/21 07:33:20  rogeeff
//  BOOST_TEST_SUPPORT_TIMEOUT flag introduced to be used by used to switch code by timeout support
//
// ***************************************************************************

#endif // BOOST_UNIT_TEST_CONFIG_HPP_071894GER
