//[example_code
#include <stdexcept>
#include <boost/test/included/prg_exec_monitor.hpp> 

int foo() { throw std::runtime_error( "big trouble" ); }

int cpp_main( int, char* [] ) // note the name
{
  foo();

  return 0;
}
//]