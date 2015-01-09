//  (C) Copyright Raffi Enficiaud 2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE example69
#include <boost/test/included/unit_test.hpp>
#include <sstream>

namespace bdata = boost::unit_test::data;


BOOST_AUTO_TEST_CASE( test_context_logging )
{
  BOOST_TEST_INFO( "some context" );
  BOOST_CHECK( false );

  int i = 12;
  BOOST_TEST_INFO( "some more context: " << i );
  BOOST_CHECK( false );

  BOOST_TEST_INFO( "info 1" );
  BOOST_TEST_INFO( "info 2" );
  BOOST_TEST_INFO( "info 3" );
  BOOST_CHECK( false );

  BOOST_TEST_CONTEXT( "some sticky context" ) 
  {
    BOOST_CHECK( false );

    BOOST_TEST_INFO( "more context" );
    BOOST_CHECK( false );

    BOOST_TEST_INFO( "different subcontext" );
    BOOST_CHECK( false );
  }

  BOOST_TEST_CONTEXT( "outer context" ) 
  {
    BOOST_CHECK( false );

    BOOST_TEST_CONTEXT( "inner context" ) 
    {
      BOOST_CHECK( false );
    }

    BOOST_CHECK( false );
  }
}
//]