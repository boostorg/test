//  test tools - test failure reported via non-zero return value  ------------//

//  (C) Copyright Beman Dawes 2000. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  Revision History
//    7 Nov 00  Initial boost version (Beman Dawes)

//  While it would appear that including boost/test_tools.hpp is unneeded,
//  it is useful because  BOOST_INCLUDE_MAIN is defined.
#define BOOST_INCLUDE_MAIN  // for testing, include rather than link
#include <boost/test/test_tools.hpp>

int test_main( int, char *[] )  // note the name
{
  return 1;
}