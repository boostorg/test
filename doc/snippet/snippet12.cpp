//[snippet12
#define __BOOST_TEST_MODULE__ MyTest
#include <boost/test/unit_test.hpp>

int add( int i, int j ) { return i+j; }

__BOOST_AUTO_TEST_CASE__( my_test )
{
  // seven ways to detect and report the same error:

  // continues on error
  __BOOST_CHECK__( add( 2,2 ) == 4 );        /*<
                                          This approach uses the __BOOST_CHECK__ tool, which displays an error message (by default on `std::cout`) that includes
                                          the expression that failed, the source file name, and the source file line number. It also increments the error
                                          count. At program termination, the error count will be displayed automatically by the Unit Test Framework.>*/
  
  // throws on error
  __BOOST_REQUIRE__( add( 2,2 ) == 4 );      /*<
                                          This approach uses the __BOOST_REQUIRE__ tool, is similar to approach #1, except that after displaying the error,
                                          an exception is thrown, to be caught by the Unit Test Framework. This approach is suitable when writing an
                                          explicit test program, and the error would be so severe as to make further testing impractical. __BOOST_REQUIRE__
                                          differs from the C++ Standard Library's assert() macro in that it is always generated, and channels error
                                          detection into the uniform Unit Test Framework reporting procedure.
                                          >*/
  
  //continues on error
  if( add( 2,2 ) != 4 )
    __BOOST_ERROR__( "Ouch..." );            /*< 
                                          This approach is similar to approach #1, except that the error detection and error reporting are coded separately.
                                          This is most useful when the specific condition being tested requires several independent statements and/or is
                                          not indicative of the reason for failure.
                                          >*/
  
  // throws on error
  if( add( 2,2 ) != 4 )
    __BOOST_FAIL__( "Ouch..." );            /*<
                                         This approach is similar to approach #2, except that the error detection and error reporting are coded separately.
                                         This is most useful when the specific condition being tested requires several independent statements and/or is
                                         not indicative of the reason for failure.
                                         >*/
  
  // throws on error
  if( add( 2,2 ) != 4 )
    throw "Ouch...";                    /*<
                                         This approach throws an exception, which will be caught and reported by the Unit Test Framework. The error
                                         message displayed when the exception is caught will be most meaningful if the exception is derived from
                                         `std::exception`, or is a `char*` or `std::string`.
                                         >*/
  
  // continues on error
  __BOOST_CHECK_MESSAGE__(add( 2,2 ) == 4,
                          "add(..) result: " << add( 2,2 ) ); /*<
                                                           This approach uses the __BOOST_CHECK_MESSAGE__ tool, is similar to approach #1, 
                                                           except that similar to the approach #3
                                                           displays an alternative error message specified as a second argument.
                                                           >*/
  
  // continues on error
  __BOOST_CHECK_EQUAL__( add( 2,2 ), 4 );   /*<
                                         This approach uses the __BOOST_CHECK_EQUAL__ tool and functionally is similar to approach #1. This approach is most
                                         attractive for checking equality of two variables, since in case of error it shows mismatched values.
                                         >*/
}
//]
