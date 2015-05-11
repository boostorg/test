//  (C) Copyright Raffi Enficiaud 2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE boost_test_macro2
#include <boost/test/included/unit_test.hpp>
#include <sstream>

BOOST_AUTO_TEST_CASE( test_compound1 )
{
  int a = 13, b = 1, c = 12;
  BOOST_TEST(a - b == c);
  BOOST_TEST(a == b + 12 == c + 1);
  BOOST_TEST(a == b + 12);
  BOOST_TEST(b + 12 == c + 1);

  BOOST_TEST(a == b + 12 == true);
  BOOST_TEST(a == b + 11 == false);
}
//]

int count = 0;
struct my_type
{
  bool operator==(my_type const& ) const { return false; }
  friend std::ostream& operator<<(std::ostream& o, my_type const& m)
  {
    o << count ++;
    return o;
  }
};

bool operator==(std::vector<my_type> const& l, std::vector<my_type> const& r)
{
  return true;
}

BOOST_AUTO_TEST_CASE( test_compound2 )
{
  std::vector<my_type> a(3);
  std::vector<my_type> b(3);
  
  BOOST_TEST(a == b);
  BOOST_TEST((a == b));
}