//[example30
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

bool is_even( int i )
{
  return i%2 == 0;
}

BOOST_AUTO_TEST_CASE( test_is_even )
{
  BOOST_CHECK_PREDICATE( is_even, (14) );

  int i = 17;
  BOOST_CHECK_PREDICATE( is_even, (i) );
}
//]