//  (C) Copyright Gennadiy Rozental 2011-2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

namespace bdata = boost::unit_test::data;


BOOST_DATA_TEST_CASE( 
  test_case_snippet_1, 
  bdata::xrange(3) * bdata::xrange(4), 
  d1, d2)
{
  //std::cout << "test 1 " << d1 << ", " << d2 << std::endl;
  BOOST_CHECK(d1 <= 3);// && d2 <= 4);
}


#if 0
BOOST_DATA_TEST_CASE( 
  test_case_snippet_2, 
  bdata::random( (bdata::distribution=std::uniform_real_distribution<float>(1, 2)) ) 
      * bdata::xrange(7),
  random_sample, d2 )
{
  std::cout << "test 2 " << random_sample << std::endl;
  BOOST_CHECK(random_sample < 1.7); // 30% chance of failure
}
#endif
//]
