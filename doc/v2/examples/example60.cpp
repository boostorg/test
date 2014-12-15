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
#include <vector>
#include <map>

namespace data = boost::unit_test::data;

std::vector<int> generate_vector()
{
  std::vector<int> out;
  out.push_back(3);
  out.push_back(1);
  out.push_back(7);
  return out;
}

typedef const std::pair<const int, int> pair_int;
BOOST_TEST_DONT_PRINT_LOG_VALUE( pair_int );

const std::vector<int> v = generate_vector();
BOOST_DATA_TEST_CASE( test_case_1, data::make(v), var1)
{
  std::cout << var1 << std::endl;
  BOOST_CHECK(true);
}


std::map<int, int> generate_map()
{
  std::vector<int> v = generate_vector();
  std::map<int, int> out;
  for(std::size_t i = 0; i < v.size(); i++)
  {
    out[v[i]] = (i * 7) % 19;
  }
  return out;
}

const std::map<int, int> m = generate_map();
BOOST_DATA_TEST_CASE( test_case_2, data::make(m), var1)
{
  std::cout << var1->first << " -- " << var1->second << std::endl;
  BOOST_CHECK(true);
}
//]
