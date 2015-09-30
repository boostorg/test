//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// @brief single header usage variant test
// ***************************************************************************

// Boost.Test
#define BOOST_TEST_MODULE single header test
#include <boost/test/included/unit_test.hpp>
using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE( test )
{
    int i = 1;
    BOOST_TEST( i*i == 1 );
}

//____________________________________________________________________________//

// EOF
