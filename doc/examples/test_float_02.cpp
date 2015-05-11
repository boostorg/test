//  (C) Copyright Andrzej Krzemienski 2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE test_float_02
#include <boost/test/included/unit_test.hpp>
namespace tt = boost::test_tools;

BOOST_AUTO_TEST_CASE(double_tolerance)
{
  double d1 = 1.00, d2 = 0.99;
  float  f1 = 1.00, f2 = 0.99;

  BOOST_TEST(d1 == d2, tt::tolerance(0.02));
  BOOST_TEST(f1 == f2, tt::tolerance(0.02));
  BOOST_TEST(f1 == d2, tt::tolerance(0.02));
  BOOST_TEST(d1 == f2, tt::tolerance(0.02));
}

BOOST_AUTO_TEST_CASE(float_tolerance)
{
  double d1 = 1.00, d2 = 0.99;
  float  f1 = 1.00, f2 = 0.99;
  
  BOOST_TEST(d1 == d2, tt::tolerance(0.02f));
  BOOST_TEST(f1 == f2, tt::tolerance(0.02f));
  BOOST_TEST(f1 == d2, tt::tolerance(0.02f));
  BOOST_TEST(d1 == f2, tt::tolerance(0.02f));
}
//]