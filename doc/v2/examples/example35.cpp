//[example_code
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test )
{
  int i = 2;
  int j = 1;
  BOOST_REQUIRE_EQUAL( i, j );
}
//]