//  (C) Copyright Andrzej Krzemienski 2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE test_float_04
#include <boost/test/included/unit_test.hpp>
namespace tt = boost::test_tools;

namespace boost { namespace math { namespace fpc {

  template <>
  struct tolerance_based<int> : boost::mpl::true_{}; 

}}}

BOOST_AUTO_TEST_CASE(test)
{
  int i1 = 1001, i2 = 1002;

  BOOST_TEST(i1 != i2);
  BOOST_TEST(i1 == i2, tt::tolerance(2));
}
//]