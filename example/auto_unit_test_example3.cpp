//  (C) Copyright Gennadiy Rozental 2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

// Boost.Test
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

BOOST_AUTO_TEST_SUITE( suite );

struct F {
    F() : i( 0 ) { BOOST_MESSAGE( "setup fixture" ); }
    ~F()         { BOOST_MESSAGE( "teardown fixture" ); }
   
    int i;
};

BOOST_FIXTURE_TEST_CASE( test1, F )
{
    BOOST_CHECK( i == 1 );
}

//____________________________________________________________________________//

BOOST_FIXTURE_TEST_CASE( test2, F )
{
    BOOST_CHECK_EQUAL( i, 2 );

    BOOST_CHECK_EQUAL( i, 0 );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE_END();

// EOF
