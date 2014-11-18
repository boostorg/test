//[example_code
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

extern void foo( int i );

BOOST_AUTO_TEST_CASE( test_external_interface )
{
  for( int i = 3; i >=0; i-- ) {
    BOOST_TEST_CHECKPOINT( "Calling foo with i=" << i );
    foo( i );
  }
}

void foo( int i )
{
    int j = 2/(i-1);
}
//]