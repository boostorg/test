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
//  Description : main function implementation for Unit Test Framework
// ***************************************************************************

#ifndef BOOST_UNIT_TEST_MAIN_IPP_012205GER
#define BOOST_UNIT_TEST_MAIN_IPP_012205GER

// Boost.Test
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test_result.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>
#include <boost/test/detail/unit_test_monitor.hpp>

// BOOST
#include <boost/scoped_ptr.hpp>

// STL
#include <iostream>                   // for cout, cerr

extern boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] );  // prototype for user's test suite init function

// ************************************************************************** //
// **************                 unit test main               ************** //
// ************************************************************************** //

int BOOST_TEST_CALL_DECL
main( int argc, char* argv[] )
{
    using namespace boost::unit_test;
    using namespace boost::unit_test::ut_detail;

    bool    no_result_code;
    bool    print_build_info;

    // set the log level
    unit_test_log.set_threshold_level_by_name( retrieve_framework_parameter( LOG_LEVEL, &argc, argv ) );

    // set the log/report format
    const_string output_format = retrieve_framework_parameter( OUTPUT_FORMAT, &argc, argv );

    if( output_format.empty() ) {
        unit_test_log.set_format( retrieve_framework_parameter( LOG_FORMAT, &argc, argv ) );
        unit_test_result::set_report_format( retrieve_framework_parameter( REPORT_FORMAT, &argc, argv ) );
    }
    else {
        unit_test_log.set_format( output_format );
        unit_test_result::set_report_format( output_format );
    }

    // set the result code and build info flags
    no_result_code    = retrieve_framework_parameter( NO_RESULT_CODE, &argc, argv ) == "no";
    print_build_info  = retrieve_framework_parameter( BUILD_INFO, &argc, argv ) == "yes";

    // set catch_system_error switch
    unit_test_monitor::catch_system_errors( retrieve_framework_parameter( CATCH_SYS_ERRORS, &argc, argv ) != "no" );

    // save report level for future reporting
    const_string report_level = retrieve_framework_parameter( REPORT_LEVEL, &argc, argv );

    // init master unit test suite
    boost::scoped_ptr<test_suite> test( init_unit_test_suite( argc, argv ) );
    if( !test ) {
        std::cerr << "*** Fail to initialize test suite" << std::endl;
        return boost::exit_test_failure;
    }

    if( retrieve_framework_parameter( DETECT_MEM_LEAKS, &argc, argv ) == "yes" )
        boost::detect_memory_leaks();

    // start testing
    unit_test_log.start( print_build_info );
    unit_test_log.header( test->size() );
    test->run();
    unit_test_log.finish( test->size() );

    // report results
    unit_test_result::instance().report( report_level, std::cerr );

    // return code
    return no_result_code ? boost::exit_success : unit_test_result::instance().result_code();
}

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.4  2005/01/31 07:50:06  rogeeff
//  cdecl portability fix
//
//  Revision 1.3  2005/01/31 06:01:54  rogeeff
//  BOOST_TEST_CALL_DECL correctness fixes
//
//  Revision 1.2  2005/01/30 01:55:13  rogeeff
//  eliminated dependency on log
//
//  Revision 1.1  2005/01/22 19:22:13  rogeeff
//  implementation moved into headers section to eliminate dependency of included/minimal component on src directory
//
//  Revision 1.18  2005/01/19 16:34:07  vawjr
//  Changed the \r\r\n back to \r\n on windows so we don't get errors when compiling
//  on VC++8.0.  I don't know why Microsoft thinks it's a good idea to call this an error,
//  but they do.  I also don't know why people insist on checking out files on Windows and
//  copying them to a unix system to check them in (which will cause exactly this problem)
//
//  Revision 1.17  2005/01/18 08:30:08  rogeeff
//  unit_test_log rework:
//     eliminated need for ::instance()
//     eliminated need for << end and ...END macro
//     straitend interface between log and formatters
//     change compiler like formatter name
//     minimized unit_test_log interface and reworked to use explicit calls
//
//  Revision 1.16  2004/06/07 07:34:23  rogeeff
//  detail namespace renamed
//
//  Revision 1.15  2004/05/21 06:26:10  rogeeff
//  licence update
//
//  Revision 1.14  2004/05/11 11:05:04  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//  namespace names shortened
//
//  Revision 1.13  2003/12/01 00:42:37  rogeeff
//  prerelease cleaning
//

// ***************************************************************************

#endif // BOOST_UNIT_TEST_MAIN_IPP_012205GER
