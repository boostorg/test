//  (C) Copyright Gennadiy Rozental 2001-2004.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : tests Unit Test Framework reporting facilities against
//  pattern file
// ***************************************************************************

// Boost.Test
#include <boost/test/unit_test_result.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>
#if !defined ( __GNUC__ ) || __GNUC__ > 2
#include <boost/test/detail/nullstream.hpp>
#endif
using boost::test_toolbox::output_test_stream;
using namespace boost::unit_test;

// BOOST
#include <boost/lexical_cast.hpp>

// STL
#include <iostream>
#include <string>
#include <utility>
#include <list>

//____________________________________________________________________________//

void good_foo() {}
void bad_foo()  {
#if !defined ( __GNUC__ ) || __GNUC__ > 2
    boost::onullstream null_stream;
#else
    output_test_stream null_stream;
#endif

    unit_test_log.set_stream( null_stream );
    BOOST_ERROR( "Sure" );
    unit_test_log.set_stream( std::cout );
}

//____________________________________________________________________________//

void check( output_test_stream& output, const_string report_format )
{
    unit_test_result::instance().set_report_format( report_format );

    unit_test_result::instance().confirmation_report( output );
    output << "*************************************************************************\n\n";
    BOOST_CHECK( output.match_pattern() );

    unit_test_result::instance().short_report( output );
    output << "*************************************************************************\n\n";
    BOOST_CHECK( output.match_pattern() );

    unit_test_result::instance().detailed_report( output );
    output << "*************************************************************************\n\n";
    BOOST_CHECK( output.match_pattern() );
}

//____________________________________________________________________________//

void check( output_test_stream& output )
{
    check( output, "HRF" );
    check( output, "XML" );
}

//____________________________________________________________________________//

int
test_main( int argc, char* argv[] )
{
    bool match_or_save = retrieve_framework_parameter( SAVE_TEST_PATTERN, &argc, argv ) != "yes";
    const_string pattern_file_name( argc > 1 ? argv[1] : "./test_files/result_report_test.pattern" );

    output_test_stream output( pattern_file_name, match_or_save );

    test_suite* ts_0 = BOOST_TEST_SUITE( "0 test cases inside" );

    test_suite* ts_1 = BOOST_TEST_SUITE( "1 test cases inside" );
        ts_1->add( BOOST_TEST_CASE( good_foo ) );

    test_suite* ts_1b = BOOST_TEST_SUITE( "1 bad test cases inside" );
        ts_1b->add( BOOST_TEST_CASE( bad_foo ), 1 );

    test_suite* ts_2 = BOOST_TEST_SUITE( "2 test cases inside" );
        ts_2->add( BOOST_TEST_CASE( good_foo ) );
        ts_2->add( BOOST_TEST_CASE( bad_foo ), 1 );

    test_suite* ts_3 = BOOST_TEST_SUITE( "3 test cases inside" );
        ts_3->add( BOOST_TEST_CASE( good_foo ) );
        ts_3->add( BOOST_TEST_CASE( good_foo ) );
        ts_3->add( BOOST_TEST_CASE( bad_foo ), 1 );

    test_suite ts_main( "Fake Test Suite Hierarchy" );
        ts_main.add( ts_0 );
        ts_main.add( ts_1 );
        ts_main.add( ts_2 );
        ts_main.add( ts_3 );

    check( output );

    ts_0->run();
    check( output );

    ts_1->run();
    check( output );

    unit_test_result::instance().increase_expected_failures();
    ts_2->run();
    check( output );

    unit_test_result::instance().increase_expected_failures();
    ts_1b->run();
    check( output );

    unit_test_result::instance().increase_expected_failures();
    ts_3->run();
    check( output );

    unit_test_result::instance().increase_expected_failures( 2 );
    ts_main.run();

    check( output );

    const_string output_format = retrieve_framework_parameter( OUTPUT_FORMAT, &argc, argv );

    if( output_format.empty() ) {
        unit_test_result::set_report_format( retrieve_framework_parameter( REPORT_FORMAT, &argc, argv ) );
    }

    return 0;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.17  2005/01/23 10:13:22  vawjr
//  Changed - \r\r\n to \r\n in the windows flavors of the files
//            VC++ 8.0 complains and won't compile them
//
//  Revision 1.16  2005/01/18 08:30:09  rogeeff
//  unit_test_log rework:
//     eliminated need for ::instance()
//     eliminated need for << end and ...END macro
//     straitend interface between log and formatters
//     change compiler like formatter name
//     minimized unit_test_log interface and reworked to use explicit calls
//
//  Revision 1.15  2004/05/27 06:30:48  rogeeff
//  no message
//
//  Revision 1.14  2004/05/21 06:26:10  rogeeff
//  licence update
//
//  Revision 1.13  2004/05/11 11:05:06  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//  namespace names shortened
//
//  Revision 1.12  2003/12/01 00:42:38  rogeeff
//  prerelease cleaning
//
// ***************************************************************************

// EOF
