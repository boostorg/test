//  (C) Copyright Gennadiy Rozental 2002-2005.
//  (C) Copyright Gennadiy Rozental & Ullrich Koethe 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

// Boost.Test
#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;

// most frequently you implement test cases as a free functions
void free_test_function()
{
    // reports 'error in "free_test_function": test 2 == 1 failed'
    BOOST_CHECK(2 == 1); // non-critical test => continue after failure

    int* p = (int*)0;
    *p = 0;
}

test_suite*
init_unit_test_suite( int, char* [] ) {
    test_suite* test= BOOST_TEST_SUITE( "Unit test example 1" );

    // this example will pass cause we know ahead of time number of expected failures
    test->add( BOOST_TEST_CASE( &free_test_function ), 1 /* expected one error */ );

    return test;
}

// EOF
