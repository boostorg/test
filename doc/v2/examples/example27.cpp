//[example27
#include <boost/test/minimal.hpp>

//____________________________________________________________________________//

int add( int i, int j ) { return i+j; }

//____________________________________________________________________________//

int test_main( int, char *[] )             // note the name!
{
  // six ways to detect and report the same error:

  // continues on error
  BOOST_CHECK( add( 2,2 ) == 4 );  /*<
                                    This approach uses the `BOOST_CHECK` tool, which displays an error message on std::cout that includes the
                                    expression that failed, the source file name, and the source file line number. It also increments the error count.
                                    At program termination, the error count will be displayed automatically by the __MTF__.
                                    >*/

  // throws on error
  BOOST_REQUIRE( add( 2,2 ) == 4 ); /*<
                                     This approach using the `BOOST_REQUIRE` tool, is similar to #1, except that after displaying the error, an
                                     exception is thrown, to be caught by the __MTF__. This approach is suitable when writing an explicit test program,
                                     and the error would be so severe as to make further testing impractical. `BOOST_REQUIRE` differs from the C++
                                     Standard Library's `assert()` macro in that it is always generated, and channels error detection into the uniform
                                     reporting procedure.
                                     >*/

  // continues on error
  if( add( 2,2 ) != 4 )
    BOOST_ERROR( "Ouch..." );       /*<
                                     This approach is similar to #1, except that the error detection is coded separately. This is most useful when
                                     the specific condition being tested is not indicative of the reason for failure.
                                     >*/
  
  // throws on error
  if( add( 2,2 ) != 4 )
    BOOST_FAIL( "Ouch..." );        /*<
                                     This approach is similar to #2, except that the error detection is coded separately. This is most useful when
                                     the specific condition being tested is not indicative of the reason for failure.
                                     >*/
  
  // throws on error
  if( add( 2,2 ) != 4 )
    throw "Oops...";                /*<
                                     This approach throws an exception, which will be caught and reported by the __MTF__. This approach is suitable
                                     for both production and test code, in libraries or not. The error message displayed when  the exception is
                                     caught  will be most meaningful if the exception is derived from `std::exception`, or is a
                                     `char*` or `std::string`.
                                     >*/

  // returns error code
  return add( 2, 2 ) == 4 ? 0 : 1;  /*<
                                     An error code may be returned by the __MTF__
                                     >*/
}
//]