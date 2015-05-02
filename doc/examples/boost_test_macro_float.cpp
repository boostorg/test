//  (C) Copyright Raffi Enficiaud 2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE boost_test_float
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_float )
{
  namespace tt = boost::test_tools;
  double d1 = 1.1e-5;
  double d2 = 1.101e-5;
  BOOST_TEST( d1 == d2, tt::tolerance( 1e-7 ) ); // Tolerance using strong comparison (Equ (2)).
  BOOST_TEST( d1 == d2, 1e-5 % tt::tolerance()); // tolerance set in percentage
}
//]
