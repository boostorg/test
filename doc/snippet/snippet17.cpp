//[snippet17
#define __BOOST_TEST_MODULE__ const_string test
#include <boost/test/unit_test.hpp>

__BOOST_AUTO_TEST_CASE__( constructors_test )
{
  //...
}

BOOST_AUTO_TEST_CASE( data_access_test )
{
  const_string cs1( "test_string" );                                 // 1 //
  __BOOST_CHECK_EQUAL__( cs1[(size_t)0], 't' );
  BOOST_CHECK_EQUAL( cs1[(size_t)4], '_' );
  BOOST_CHECK_EQUAL( cs1[cs1.length()-1], 'g' );

  BOOST_CHECK_EQUAL( cs1[(size_t)0], cs1.at( 0 ) );                  // 2 //
  BOOST_CHECK_EQUAL( cs1[(size_t)2], cs1.at( 5 ) );
  BOOST_CHECK_EQUAL( cs1.at( cs1.length() - 1 ), 'g' );

  BOOST_CHECK_THROW( cs1.at( cs1.length() ), std::out_of_range );    // 3 //
}
//]
