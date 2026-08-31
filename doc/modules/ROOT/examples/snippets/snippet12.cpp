//  (C) Copyright Gennadiy Rozental 2001-2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/test for the library home page.
//

// tag::snippet12[]
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

int add( int i, int j ) { return i + j; }

BOOST_AUTO_TEST_CASE(my_test)
{
  // six ways to detect and report the same error:

  // continues on error
  BOOST_TEST( add(2, 2) == 4 ); <1>
  
  // throws on error
  BOOST_TEST_REQUIRE( add(2, 2) == 4 ); <2>
  
  //continues on error
  if (add(2, 2) != 4)
    BOOST_ERROR( "Ouch..." ); <3>
  
  // throws on error
  if (add(2, 2) != 4)
    BOOST_FAIL( "Ouch..." ); <4>
  
  // throws on error
  if (add(2, 2) != 4)
    throw "Ouch..."; <5>
  
  // continues on error
  BOOST_TEST( add(2, 2) == 4, <6>
              "2 plus 2 is not 4 but " << add(2, 2));
}
// end::snippet12[]

/* tag::snippet12-callouts[]
<1> This approach uses tool BOOST_TEST, which displays an error message (by default on `std::cout`) that includes the expression that failed, as well as the values on the two side of the equation, the source file name, and the source file line number. It also increments the error count. At program termination, the error count will be displayed automatically by the Unit Test Framework.
<2> This approach uses tool BOOST_TEST_REQUIRE, is similar to approach #1, except that after displaying the error, an exception is thrown, to be caught by the Unit Test Framework. This approach is suitable when writing an explicit test program, and the error would be so severe as to make further testing impractical.
<3> This approach is similar to approach #1, except that the error detection and error reporting are coded separately. This is most useful when the specific condition being tested requires several independent statements and/or is not indicative of the reason for failure.
<4> This approach is similar to approach #2, except that the error detection and error reporting are coded separately. This is most useful when the specific condition being tested requires several independent statements and/or is not indicative of the reason for failure.
<5> This approach throws an exception, which will be caught and reported by the Unit Test Framework. The error message displayed when the exception is caught will be most meaningful if the exception is derived from `std::exception`, or is a `char*` or `std::string`.
<6> This approach uses tool BOOST_TEST with additional message argument, is similar to approach #1, except that similar to the approach #3 displays an alternative error message specified as a second argument.
end::snippet12-callouts[] */
