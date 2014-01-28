//[example46
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test )
{
  BOOST_ERROR( "Nothing to test" );
}
//]