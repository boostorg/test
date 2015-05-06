//  (C) Copyright Raffi Enficiaud 2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE boost_test_sequence
#include <boost/test/included/unit_test.hpp>
#include <vector>

BOOST_AUTO_TEST_CASE( test_collections_vectors )
{
  std::vector<int> a{1,2,3}, c{1,5,3,4};
  std::vector<long> b{1,5,3};
  
  BOOST_TEST(a == b); // nok: a[1] != b[1]
  BOOST_TEST(a != b); // nok: a[0] == b[0] ...
  BOOST_TEST(a <= b); // ok
  BOOST_TEST(b < c);  // nok: size mismatch
  BOOST_TEST(b >= c); // nok: size mismatch
  BOOST_TEST(b != c); // nok: size mismatch
}
//]
