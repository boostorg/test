//  (C) Copyright Gennadiy Rozental 2002-2003.
//  (C) Copyright Gennadiy Rozental & Ullrich Koethe 2001.
//  See accompanying license for terms and conditions of use.
//
//  See http://www.boost.org for most recent version including documentation.

// Boost.Test
#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;

// most frequently you implement test cases as a free functions
void free_test_function()
{
    // reports 'error in "free_test_function": test 2 == 1 failed'
    BOOST_CHECK(2 == 1); // non-critical test => continue after failure
}

test_suite*
init_unit_test_suite( int, char* [] ) {
    test_suite* test= BOOST_TEST_SUITE( "Unit test example 1" );

    // this example will pass cause we know ahead of time number of expected failures
    test->add( BOOST_TEST_CASE( &free_test_function ), 1 /* expected one error */ );

    return test;
}

// EOF
