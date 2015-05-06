//  (C) Copyright Raffi Enficiaud 2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE boost_test_sequence_float
#include <boost/test/included/unit_test.hpp>
#include <vector>

BOOST_AUTO_TEST_CASE( test_collections_vectors_floats )
{
  std::vector<float> a{0.1f, 0.2f, 0.3f};
  std::vector<double> b{0.11, 0.2, 0.29};
  
  BOOST_TEST(a == b); // nok: a[0] != b[0]
  BOOST_TEST(a == b, boost::test_tools::tolerance<float>(3000)); // not working
}


BOOST_AUTO_TEST_CASE( test_collections_vectors2 )
{
  std::vector<int> a(3);
  std::vector<long> b(3);
  
  a[0] = 1; a[1] = 2; a[2] = 3;
  b[0] = 1; b[1] = 5; b[2] = 3;
  BOOST_TEST(a == b); // nok: a[1] != b[1]
  BOOST_TEST(a != b); // nok: a[0] == b[0] ...
  BOOST_TEST(a == b == true); // ok
}
//]
