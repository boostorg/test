//  boost cpp_main_example program  ------------------------------------------//

#include <boost/test/cpp_main.cpp> // included rather than linked implementation

int add( int i, int j ) { return i+j; }

int cpp_main( int, char *[] )  // note the name!
{
  // two ways to detect and report the same error:
  if ( add(2,2) != 4 ) throw "Oops..."; // #1 throws on error
  return add(2,2) == 4 ? 0 : 1;         // #2 returns error directly
}
