//  (C) Copyright Gennadiy Rozental 2002-2004.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : tests parameterized tests
// ***************************************************************************

// Boost.Test
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test_result.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_log.hpp>
#if BOOST_WORKAROUND(  __GNUC__, < 3 )
#include <boost/test/output_test_stream.hpp>
typedef boost::test_tools::output_test_stream onullstream_type;
#else
#include <boost/test/utils/nullstream.hpp>
typedef boost::onullstream onullstream_type;
#endif

namespace utf=boost::unit_test;

// BOOST
#include <boost/scoped_ptr.hpp>

// STL
#include <list>
#include <iostream>

//____________________________________________________________________________//

void test0( int i )
{
    BOOST_CHECK( i%2 == 0 );
}

//____________________________________________________________________________//

void test1( int i )
{
    BOOST_CHECK( i%2 == 0 );
    if( i%3 == 0 ) {
        throw 124;
    }
}

//____________________________________________________________________________//


#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x570))
#define BOOST_PARAM_TEST_CASE__( arg1, arg2, arg3 ) \
    boost::unit_test::create_test_case<int*,int>( (arg1), std::string( "" ), (arg2), (arg3) )
#else
#define BOOST_PARAM_TEST_CASE__( arg1, arg2, arg3 ) BOOST_PARAM_TEST_CASE( arg1, arg2, arg3 )
#endif

int test_main( int, char* [] ) {
    utf::counter_t                      num_of_failures;
    bool                                exception_caught;
    bool                                passed;
    onullstream_type                    null_output;
    boost::scoped_ptr<utf::test_case>   test;  

    utf::unit_test_log.set_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 2, 2, 2 };
    test.reset( BOOST_PARAM_TEST_CASE__( &test0, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    }

    utf::unit_test_log.set_stream( std::cout );
    BOOST_CHECK( num_of_failures == 0 );
    BOOST_CHECK( !exception_caught );

    utf::unit_test_log.set_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 1, 2, 2 };    
    test.reset( BOOST_PARAM_TEST_CASE__( &test0, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    }

    utf::unit_test_log.set_stream( std::cout );
    BOOST_CHECK( num_of_failures == 1 );
    BOOST_CHECK( !exception_caught );

    utf::unit_test_log.set_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 1, 1, 2 };    
    test.reset( BOOST_PARAM_TEST_CASE__( &test0, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    }

    utf::unit_test_log.set_stream( std::cout );
    BOOST_CHECK_EQUAL( num_of_failures, (std::size_t)2 );
    BOOST_CHECK( !exception_caught );

    utf::unit_test_log.set_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 1, 1, 1 };    
    test.reset( BOOST_PARAM_TEST_CASE__( &test0, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    }

    utf::unit_test_log.set_stream( std::cout );
    BOOST_CHECK_EQUAL( num_of_failures, (std::size_t)3 );
    BOOST_CHECK( !exception_caught );

    utf::unit_test_log.set_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 6, 6, 6 };    
    test.reset( BOOST_PARAM_TEST_CASE__( &test1, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    passed = utf::unit_test_result::instance().has_passed();
    }

    utf::unit_test_log.set_stream( std::cout );
    BOOST_CHECK_EQUAL( num_of_failures, (std::size_t)0 );
    BOOST_CHECK( !exception_caught );
    BOOST_CHECK( !passed );

    utf::unit_test_log.set_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 0, 3, 9 };    
    test.reset( BOOST_PARAM_TEST_CASE__( &test1, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    }

    utf::unit_test_log.set_stream( std::cout );
    BOOST_CHECK_EQUAL( num_of_failures, (std::size_t)2 );
    BOOST_CHECK( !exception_caught );

    utf::unit_test_log.set_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 2, 3, 9 };
    test.reset( BOOST_PARAM_TEST_CASE__( &test1, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    }

    utf::unit_test_log.set_stream( std::cout );
    BOOST_CHECK_EQUAL( num_of_failures, (std::size_t)2 );
    BOOST_CHECK( !exception_caught );

    utf::unit_test_log.set_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 3, 2, 6 };    
    test.reset( BOOST_PARAM_TEST_CASE__( &test1, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    }

    utf::unit_test_log.set_stream( std::cout );
    BOOST_CHECK_EQUAL( num_of_failures, (std::size_t)1 );
    BOOST_CHECK( !exception_caught );

    return 0;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.18  2005/01/31 20:01:39  rogeeff
//  use BOOST_WORKAROUND
//
//  Revision 1.17  2005/01/30 03:35:55  rogeeff
//  no message
//
//  Revision 1.15  2005/01/18 08:30:09  rogeeff
//  unit_test_log rework:
//     eliminated need for ::instance()
//     eliminated need for << end and ...END macro
//     straitend interface between log and formatters
//     change compiler like formatter name
//     minimized unit_test_log interface and reworked to use explicit calls
//
//  Revision 1.14  2004/05/21 06:26:10  rogeeff
//  licence update
//
//  Revision 1.13  2004/05/11 11:05:06  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//  namespace names shortened
//
//  Revision 1.12  2003/12/03 01:22:40  rogeeff
//  *** empty log message ***
//
//  Revision 1.11  2003/12/01 00:42:37  rogeeff
//  prerelease cleaning
//

// ***************************************************************************

// EOF
