//  (C) Copyright Gennadiy Rozental 2011-2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/bind.hpp>
using namespace boost::unit_test;

void free_test_function( int i, int j )
{
  BOOST_CHECK( true /* test assertion */ );
}

bool init_function()
{
  framework::master_test_suite().
    add( BOOST_TEST_CASE( boost::bind( &free_test_function, 1, 1 ) ) );
  framework::master_test_suite().
    add( BOOST_TEST_CASE( boost::bind( &free_test_function, 1, 2 ) ) );
  framework::master_test_suite().
    add( BOOST_TEST_CASE( boost::bind( &free_test_function, 2, 1 ) ) );

  return true;
}

int main( int argc, char* argv[] )
{
  return ::boost::unit_test::unit_test_main( &init_function, argc, argv );
}
//]
