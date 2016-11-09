//  (C) Copyright Raffi Enficiaud 2016.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  Checking issue https://svn.boost.org/trac/boost/ticket/11962
// ***************************************************************************

// Boost.Test
#define BOOST_TEST_MODULE loggers_support_properly_messages_in_fixtures
#include <boost/test/included/unit_test.hpp>

struct GlobalFixture {

  GlobalFixture() {
     BOOST_TEST_MESSAGE("this is the message");
  }
};

BOOST_GLOBAL_FIXTURE( GlobalFixture );

BOOST_AUTO_TEST_CASE( test_case )
{
  BOOST_TEST( true );
}
