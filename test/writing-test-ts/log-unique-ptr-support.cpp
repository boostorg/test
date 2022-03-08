//  (C) Copyright Marek Kurdej 2019.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! std::unique_ptr support check
// *****************************************************************************

#define BOOST_TEST_MODULE std::unique_ptr support
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <vector>

struct dummy_class {
    dummy_class(int i = 0): value(i) {}
    operator bool() const { return bool(value); }

    bool operator==(dummy_class const &r) const { return value == r.value;  }
    bool operator!=(dummy_class const &r) const { return value != r.value;; }
    
    int value;
};

BOOST_TEST_DONT_PRINT_LOG_VALUE(dummy_class)

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE(single_object)
{
  dummy_class actual, expected;
  BOOST_TEST(actual == expected);

  std::unique_ptr<dummy_class> nothing(nullptr);
  BOOST_TEST(nothing == nullptr);

    std::unique_ptr<dummy_class> d1(new dummy_class(1));
    std::unique_ptr<dummy_class> d2(new dummy_class(1));
  BOOST_TEST(d1 == d2);
}

//____________________________________________________________________________//

// this one tests for contexts printing in dataset tests
std::unique_ptr<dummy_class> generate_ptr()
{
  return std::make_unique_ptr(new dummy_class());
}

//____________________________________________________________________________//

BOOST_DATA_TEST_CASE( test_data_case, boost::unit_test::data::make(generate_ptr()))
{
  BOOST_TEST(sample);
}

// EOF
