//  (C) Copyright Gennadiy Rozental 2002-2003.
//  See accompanying license for terms and conditions of use.

//  See http://www.boost.org for most recent version including documentation.
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
#if !defined ( __GNUC__ ) || __GNUC__ > 2
#include <boost/test/detail/nullstream.hpp>
typedef boost::onullstream onullstream_type;
#else
typedef boost::test_toolbox::output_test_stream onullstream_type;
#endif

namespace utf=boost::unit_test_framework;

#include <list>
#include <iostream>
#include <boost/scoped_ptr.hpp>

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


#if defined(__BORLANDC__) && (__BORLANDC__ < 0x570)
#define BOOST_PARAM_TEST_CASE__( arg1, arg2, arg3 ) \
    boost::unit_test_framework::create_test_case<int*,int>( (arg1), std::string( "" ), (arg2), (arg3) )
#else
#define BOOST_PARAM_TEST_CASE__( arg1, arg2, arg3 ) BOOST_PARAM_TEST_CASE( arg1, arg2, arg3 )
#endif

int test_main( int, char* [] ) {
    utf::unit_test_counter              num_of_failures;
    bool                                exception_caught;
    bool                                passed;
    onullstream_type                    null_output;
    boost::scoped_ptr<utf::test_case>   test;  

    utf::unit_test_log::instance().set_log_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 2, 2, 2 };
    test.reset( BOOST_PARAM_TEST_CASE__( &test0, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    }

    utf::unit_test_log::instance().set_log_stream( std::cout );
    BOOST_CHECK( num_of_failures == 0 );
    BOOST_CHECK( !exception_caught );

    utf::unit_test_log::instance().set_log_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 1, 2, 2 };    
    test.reset( BOOST_PARAM_TEST_CASE__( &test0, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    }

    utf::unit_test_log::instance().set_log_stream( std::cout );
    BOOST_CHECK( num_of_failures == 1 );
    BOOST_CHECK( !exception_caught );

    utf::unit_test_log::instance().set_log_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 1, 1, 2 };    
    test.reset( BOOST_PARAM_TEST_CASE__( &test0, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    }

    utf::unit_test_log::instance().set_log_stream( std::cout );
    BOOST_CHECK_EQUAL( num_of_failures, (std::size_t)2 );
    BOOST_CHECK( !exception_caught );

    utf::unit_test_log::instance().set_log_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 1, 1, 1 };    
    test.reset( BOOST_PARAM_TEST_CASE__( &test0, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    }

    utf::unit_test_log::instance().set_log_stream( std::cout );
    BOOST_CHECK_EQUAL( num_of_failures, (std::size_t)3 );
    BOOST_CHECK( !exception_caught );

    utf::unit_test_log::instance().set_log_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 6, 6, 6 };    
    test.reset( BOOST_PARAM_TEST_CASE__( &test1, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    passed = utf::unit_test_result::instance().has_passed();
    }

    utf::unit_test_log::instance().set_log_stream( std::cout );
    BOOST_CHECK_EQUAL( num_of_failures, (std::size_t)0 );
    BOOST_CHECK( !exception_caught );
    BOOST_CHECK( !passed );

    utf::unit_test_log::instance().set_log_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 0, 3, 9 };    
    test.reset( BOOST_PARAM_TEST_CASE__( &test1, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    }

    utf::unit_test_log::instance().set_log_stream( std::cout );
    BOOST_CHECK_EQUAL( num_of_failures, (std::size_t)2 );
    BOOST_CHECK( !exception_caught );

    utf::unit_test_log::instance().set_log_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 2, 3, 9 };
    test.reset( BOOST_PARAM_TEST_CASE__( &test1, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    }

    utf::unit_test_log::instance().set_log_stream( std::cout );
    BOOST_CHECK_EQUAL( num_of_failures, (std::size_t)2 );
    BOOST_CHECK( !exception_caught );

    utf::unit_test_log::instance().set_log_stream( null_output );
    {
    utf::unit_test_result_saver saver;
    int test_data[] = { 3, 2, 6 };    
    test.reset( BOOST_PARAM_TEST_CASE__( &test1, (int*)test_data, (int*)test_data + sizeof(test_data)/sizeof(int) ) );

    test->run();

    utf::unit_test_result::instance().failures_details( num_of_failures, exception_caught );
    }

    utf::unit_test_log::instance().set_log_stream( std::cout );
    BOOST_CHECK_EQUAL( num_of_failures, (std::size_t)1 );
    BOOST_CHECK( !exception_caught );

    return 0;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.9  2003/10/27 07:13:32  rogeeff
//  licence update
//
//  Revision 1.8  2003/06/12 00:02:39  rogeeff
//  *** empty log message ***
//
//  Revision 1.7  2003/06/11 15:47:03  rogeeff
//  eliminate mismatch types warning
//
//  Revision 1.6  2003/06/10 07:57:33  rogeeff
//  test_case_template_test added
//  parameterized test updated
//
//  Revision 1.5  2003/06/09 09:25:24  rogeeff
//  1.30.beta1
//
//  Revision 1.4  2003/02/15 21:52:37  rogeeff
//  mingw ostream fix
//
//  Revision 1.3  2002/12/09 05:16:10  rogeeff
//  switched to use utf::unit_test_result_saver for internal testing
//
//  Revision 1.2  2002/11/02 20:04:43  rogeeff
//  release 1.29.0 merged into the main trank
//

// ***************************************************************************

// EOF
