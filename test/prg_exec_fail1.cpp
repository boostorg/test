//  Boost Program Execution Monitor test 1 (should fail) ---------------------//

//  (C) Copyright Beman Dawes, Gennadiy Rozental 2001. Permission to copy, use,
//  modify, sell and distribute this software is granted provided this copyright
//  notice appears in all copies. This software is provided "as is" without express
//  or implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

int cpp_main( int, char *[] )  // note the name
{
  throw "Test error by throwing C-style string exception";
  return 0;
}
