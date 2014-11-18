//[example_code
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

class my_exception{};

BOOST_AUTO_TEST_CASE( test )
{
  BOOST_CHECK_NO_THROW( throw my_exception() );
}
//]