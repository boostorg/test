//  test tools - test failure reported by throwing exception  ----------------//

//  (C) Copyright Beman Dawes 2001. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  Revision History
//    8 Feb 01  Initial boost version (Beman Dawes)

#include <boost/test/test_tools.hpp>

int test_main( int, char *[] )  // note the name
{
  throw "Test error by throwing C-style string exception";
  return 0;
}