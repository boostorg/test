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
  
  BOOST_TEST(a == b); // fails
  BOOST_TEST(a == b, 10. % boost::test_tools::tolerance()); // pass
}

//]
