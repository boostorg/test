//  test execution monitor - test failure reported by both macros ------------//

//  (C) Copyright Beman Dawes 2000, Gennadiy Rozental 2001. Permission to copy, 
//  use, modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided "as is"
//  without express or implied warranty, and with no claim as to its suitability 
//  for any purpose.

#include <boost/test/test_tools.hpp>

int test_main( int, char *[] )  // note the name
{
  int v = 1;
  BOOST_TEST( v == 2 );
  BOOST_ERROR( "sample BOOST_ERROR call" );
  BOOST_CRITICAL_TEST( 2 == v );
  throw "Opps! should never reach this point";
  return 1;
}

//  Revision History
//    7 Nov 00  Initial boost version (Beman Dawes)
//   25 Oct 01  Revisited version (Gennadiy Rozental)

// EOF
