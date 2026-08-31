//  (C) Copyright Gennadiy Rozental 2001-2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/test for the library home page.
//

// tag::snippet17[]
#define BOOST_TEST_MODULE const_string test
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( constructors_test )
{
  //...
}

BOOST_AUTO_TEST_CASE( data_access_test )
{
  const_string cs1( "test_string" );                                // 1 //
  BOOST_TEST( cs1[(size_t)0] == 't' );
  BOOST_TEST( cs1[(size_t)4] == '_' );
  BOOST_TEST( cs1[cs1.length()-1] == 'g' );

  BOOST_TEST( cs1[(size_t)0] == cs1.at( 0 ) );                      // 2 //
  BOOST_TEST( cs1[(size_t)2] == cs1.at( 5 ) );
  BOOST_TEST( cs1.at( cs1.length() - 1 ) == 'g' );

  BOOST_CHECK_THROW( cs1.at( cs1.length() ), std::out_of_range );   // 3 //
}
// end::snippet17[]
