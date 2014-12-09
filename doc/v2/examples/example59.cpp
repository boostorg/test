//[example_code
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

BOOST_DATA_TEST_CASE( test_case_snippet_1, boost::unit_test::data::xrange(5) )
{
  std::cout << "test 1 " << sample << std::endl;
  BOOST_CHECK(sample <= 4 && sample >= 0);
}

BOOST_DATA_TEST_CASE( test_case_snippet_2, boost::unit_test::data::xrange<int>( (data::begin=1, data::end=10, data::step=3)) )
{
  std::cout << "test 2 " << sample << std::endl;
  BOOST_CHECK(sample <= 4 && sample >= 0);
}

//]

