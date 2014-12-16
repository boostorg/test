//  (C) Copyright Marek Kurdej 2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! BOOST_TEST_DONT_PRINT_LOG_VALUE unit test
// *****************************************************************************

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <vector>

struct dummy_class
{
  dummy_class(int i = 0) : i_(i){}

  bool operator==(dummy_class const&) const
  {
    return true;
  }
    
  bool operator!=(dummy_class const&) const
  {
    return false;
  }

  int i_;
};

typedef ::std::vector<dummy_class> vector_dummy_class;

BOOST_TEST_DONT_PRINT_LOG_VALUE(dummy_class)

BOOST_AUTO_TEST_CASE(single_object)
{
  dummy_class actual, expected;
  BOOST_CHECK_EQUAL(actual, expected);
}

BOOST_AUTO_TEST_CASE(collection_of_objects)
{
  vector_dummy_class actual, expected;
  BOOST_CHECK_EQUAL_COLLECTIONS(actual.begin(), actual.end(), expected.begin(), expected.end());
}

// this one tests for contexts
vector_dummy_class generate_vector()
{
  vector_dummy_class out;
  out.push_back(3);
  out.push_back(1);
  out.push_back(7);
  return out;
}

vector_dummy_class v = generate_vector();
BOOST_DATA_TEST_CASE( test_data_case, boost::unit_test::data::make(v), var1)
{
  BOOST_CHECK(true);
}
