//  (C) Copyright Andrzej Krzemienski 2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE decorator_06
#include <boost/test/included/unit_test.hpp>

namespace utf = boost::unit_test;
namespace tt = boost::test_tools;

BOOST_AUTO_TEST_CASE(test1)
{
    BOOST_TEST( 1 == 1 );
}

BOOST_AUTO_TEST_CASE(test2)
{
    BOOST_TEST( 2 != 2 );
}

struct if_either
{
  std::string tc1, tc2;
  if_either(std::string t1, std::string t2) : tc1(t1), tc2(t2) {}
  
  tt::assertion_result operator()(utf::test_unit_id)
  {
    auto& master = utf::framework::master_test_suite();
    auto& collector = utf::results_collector_t::instance();
    auto& test1_result = collector.results(master.get(tc1));
    auto& test2_result = collector.results(master.get(tc2));

    return test1_result.passed() || test2_result.passed();
  }
};

BOOST_AUTO_TEST_CASE(test3, *utf::precondition(if_either("test1", "test2")))
{
    BOOST_TEST( 3 != 3 );
}

BOOST_AUTO_TEST_CASE(test4, *utf::precondition(if_either("test2", "test3")))
{
    BOOST_TEST( 4 != 4 );
}
//]