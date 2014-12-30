#define BOOST_TEST_MODULE My Test
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(first_test)
{
  BOOST_TEST(true);

  int i = 1;
  BOOST_TEST(i == 1);
}
