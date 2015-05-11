//  (C) Copyright Andrzej Krzemienski 2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE test_float_01
#include <boost/test/included/unit_test.hpp>
namespace tt = boost::test_tools;
namespace fpc = boost::test_tools::fpc;

BOOST_AUTO_TEST_CASE(test)
{
  double d1 = 1.00;
  double d2 = 0.99;
  
  BOOST_TEST(d1 == d2);
  
  BOOST_TEST(d1 == d2, tt::tolerance(0.02));
    
  BOOST_TEST(d1 == d2, 2.0 % tt::tolerance());
    
  BOOST_TEST(d1 == d2,
    tt::tolerance(fpc::percent_tolerance(2.0)));
}
//]