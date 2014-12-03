#define BOOST_TEST_MODULE my_first_test
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(first_test)
{
  BOOST_CHECK(true);
  
  int i = 1;
  BOOST_CHECK_EQUAL(i, 1);
}

