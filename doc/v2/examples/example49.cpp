//[example_code
#include <boost/test/included/unit_test.hpp>
using namespace boost::unit_test;

void free_test_function()
{
  BOOST_CHECK( true /* test assertion */ );
}

test_suite* init_unit_test_suite( int argc, char* argv[] ) 
{
  for( int i=0; i < 10000; i++ )
    framework::master_test_suite().
      add( BOOST_TEST_CASE( &free_test_function ) );

  return 0;
}
//]