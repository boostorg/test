//  (C) Copyright Raffi Enficiaud 2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE boost_test_macro_float2
#include <boost/test/included/unit_test.hpp>
#include <sstream>

BOOST_AUTO_TEST_CASE( test_floating_points )
{
  namespace tt = boost::test_tools;
  int a = 13;
  BOOST_TEST(float(a) == float(a + 1), tt::tolerance<float>( 1./13 ));
  
  // tolerance not properly set for floating points
  BOOST_TEST(float(a) == float(a + 1), tt::tolerance( 1./13 ));
}
//]
