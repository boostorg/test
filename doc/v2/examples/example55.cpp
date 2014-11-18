//[example_code
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test )
{
  int i = 9;
  int j = 6;
  BOOST_CHECK_LE( i, j );
}
//]