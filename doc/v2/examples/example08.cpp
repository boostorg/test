//  (C) Copyright Gennadiy Rozental 2011-2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
using namespace boost::unit_test;
using namespace boost;

class test_class
{
public:
  void test_method( double const& d )
  {
    BOOST_CHECK_CLOSE( d * 100, (double)(int)(d*100), 0.01 );
  }
} tester;

bool init_unit_test()
{
  double params[] = { 1., 1.1, 1.01, 1.001, 1.0001 };

  callback1<double> tm = bind( &test_class::test_method, &tester, _1);

  framework::master_test_suite().
    add( BOOST_PARAM_TEST_CASE( tm, params, params+5 ) );

  return true;
}
//]
