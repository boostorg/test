//  (C) Copyright Andrzej Krzemienski 2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE test_float_03
#include <boost/test/included/unit_test.hpp>
#include <boost/optional/optional_io.hpp>
namespace tt = boost::test_tools;

BOOST_AUTO_TEST_CASE(test)
{
  double                  d1 = 1.00, d2 = 0.99;
  boost::optional<double> o1 = 1.00, o2 = 0.99;

  BOOST_TEST(d1 == d2, tt::tolerance(0.02));
  BOOST_TEST(o1 == o2, tt::tolerance(0.02));
  BOOST_TEST(o1 == d2, tt::tolerance(0.02));
  BOOST_TEST(d1 == o2, tt::tolerance(0.02));
}
//]