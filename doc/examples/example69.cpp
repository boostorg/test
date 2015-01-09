//  (C) Copyright Raffi Enficiaud 2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#define BOOST_TEST_MODULE example69
#include <boost/test/included/unit_test.hpp>
#include <sstream>


void func2()
{
  BOOST_TEST_INFO( "[context] func2" );
  BOOST_CHECK( false );
}

void func1()
{
  BOOST_TEST_INFO( "[context] func1" );
  func2();
}

void func3(int i)
{
  if(i < 0)
  {
    BOOST_TEST_CONTEXT( "[context] i negative branch") 
    {
      func2();
      return;
    }
  }
  
  BOOST_TEST(i != 1);
}

BOOST_AUTO_TEST_CASE( test_context_logging )
{
  BOOST_TEST_INFO( "context consumed by the next assertion" );
  BOOST_CHECK( true );

  int a = 1;
  int b = 1;
  BOOST_TEST_INFO( "checking a != b [" << a << "!=" << b << "]" );
  BOOST_TEST( a != b );


  BOOST_TEST_INFO( "stacking" );
  func1();
  
  // no context, consumed by the test in func2
  BOOST_CHECK( false );
  
  BOOST_TEST_INFO( "*** consumed by first assertion (failing or not) ***" );
  BOOST_TEST_CONTEXT( "[context] shown every time (sticky)") 
  {
    for(int i = -1; i < 2; i++)
    {
      BOOST_TEST_CONTEXT( "[context] calling func3( " << i << " )")
      {
        func3(i);
      }
    }
  }

}
//]