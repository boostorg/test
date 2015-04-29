//  (C) Copyright Raffi Enficiaud 2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE boost_test_collections
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <map>
#include <vector>

BOOST_AUTO_TEST_CASE( test_collections_not_on_c_arrays )
{
  int a[] = {1, 2, 3};
  int b[] = {1, 5, 3, 4};
  BOOST_TEST(a == b);
}

BOOST_AUTO_TEST_CASE( test_collections_maps )
{
  std::map<int, int> a;
  std::map<long, int> b;
  
  a[0] = 1; 
  b[0] = 1; 
  
  // std::map<int, int>::iterator::reference does not support ==
  // BOOST_TEST(a == b);
}

BOOST_AUTO_TEST_CASE( test_collections_vectors )
{
  std::vector<int> a(3), c(4);
  std::vector<long> b(3);
  
  a[0] = 1; a[1] = 2; a[2] = 3;
  b[0] = 1; b[1] = 5; b[2] = 3;
  c[0] = 1; c[1] = 5; c[2] = 3; c[3] = 4;
  BOOST_TEST(a == b); // nok: a[1] != b[1]
  BOOST_TEST(a != b); // nok: a[0] == b[0] ...
  BOOST_TEST(a <= b); // ok
  BOOST_TEST(b < c);  // nok: size mismatch
  BOOST_TEST(b != c); // nok: size mismatch
}
//]

