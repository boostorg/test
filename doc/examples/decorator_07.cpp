//  (C) Copyright Andrzej Krzemienski 2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE decorator_07
#include <boost/test/included/unit_test.hpp>

namespace utf = boost::unit_test;

BOOST_TEST_DECORATOR(
 - utf::disabled()
 - utf::description("enable only when ODBC is available")
)
BOOST_AUTO_TEST_CASE(test_1)
{
  BOOST_TEST(1 == 1);
}

BOOST_TEST_DECORATOR(
 - utf::description("descriptions ")
 - utf::description("add up")
)
BOOST_AUTO_TEST_CASE(test_2)
{
  BOOST_TEST(2 == 2);
}
//]
