//[example_code
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test )
{
  int i = 3;
  int j = 3;
  BOOST_CHECK_NE( i, j );
}
//]