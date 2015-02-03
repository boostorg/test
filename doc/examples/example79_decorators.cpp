//  (C) Copyright Andrzej Krzemienski 2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#include <iostream>
#define BOOST_TEST_MODULE example79
#include <boost/test/unit_test.hpp>

namespace utf = boost::unit_test;

struct Fx
{
  std::string s;
  Fx(std::string s = "") : s(s)
        { std::cout << "set up " << s << std::endl; }
  ~Fx() { std::cout << "tear down " << s << std::endl; }
};

void setup() { std::cout << "set up fun" << std::endl; }
void teardown() { std::cout << "tear down fun" << std::endl; }

BOOST_TEST_DECORATOR(
  + utf::fixture<Fx>(std::string("FX"))
  + utf::fixture<Fx>(std::string("FX2"))
)
BOOST_AUTO_TEST_SUITE( suite1 )

  BOOST_TEST_DECORATOR(
    + utf::fixture(&setup, &teardown)
  )
  BOOST_AUTO_TEST_CASE( test_case1 )
  {
    BOOST_TEST(false);
  }
  
  BOOST_AUTO_TEST_CASE( test_case2 )
  {
    BOOST_TEST(false);
  }

BOOST_AUTO_TEST_SUITE_END()
//]
