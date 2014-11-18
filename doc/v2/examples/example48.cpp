//[example_code
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test )
{
  BOOST_CHECK( BOOST_IS_DEFINED(SYMBOL1) );
  BOOST_CHECK( BOOST_IS_DEFINED(SYMBOL2(arg)) );
}
//]