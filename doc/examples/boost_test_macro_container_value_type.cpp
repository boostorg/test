//  (C) Copyright Raffi Enficiaud 2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE boost_test_sequence_value_t
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <map>
#include <vector>

BOOST_AUTO_TEST_CASE( test_collections_maps )
{
  std::map<int, int> a;
  std::map<long, int> b;
  
  a[0] = 1; 
  b[0] = 1; 
  
  // std::map<int, int>::iterator::reference does not 
  // implement operator== out of the box.
  // BOOST_TEST(a == b);
}
//]
