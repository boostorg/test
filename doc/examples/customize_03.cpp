//  (C) Copyright Andrzej Krzemienski 2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE customize_03 /*<module name is also master test suite's name>*/
#define BOOST_TEST_NO_MAIN /*<suppress entry point auto generation>*/
#define BOOST_TEST_ALTERNATIVE_INIT_API /*<recommended initialization API>*/
#include <boost/test/included/unit_test.hpp>
#include <boost/test/tree/visitor.hpp>
#include <iostream>
namespace utf = boost::unit_test;

struct ListSuites : utf::test_tree_visitor
{
  bool test_suite_start(const utf::test_suite& s)
  {
    std::cout << s.p_name.get() << std::endl; /*<display test suite's name>*/
    return true;
  }
};

int test_runner(utf::init_unit_test_func init_func, int argc, char* argv[])
{
  try {
    utf::framework::init(init_func, argc, argv);
    utf::framework::finalize_setup_phase();
    ListSuites ls;
    traverse_test_tree(utf::framework::master_test_suite(), ls, true); /*<visit test tree>*/
  }
  catch (...) { return EXIT_FAILURE; }

  utf::framework::shutdown();
  return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) /*<custom entry point>*/
{
  return test_runner(init_unit_test, argc, argv);
}

BOOST_AUTO_TEST_SUITE(suiteA)
  BOOST_AUTO_TEST_CASE(test1){}
  BOOST_AUTO_TEST_CASE(test2){}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(suiteB)
  BOOST_AUTO_TEST_CASE(test1){}
  BOOST_AUTO_TEST_CASE(test2){}
BOOST_AUTO_TEST_SUITE_END()
//]