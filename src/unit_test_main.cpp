//  (C) Copyright Gennadiy Rozental 2001-2002.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.
//
//  File        : $RCSfile$
//
//  Version     : $Id$
//
//  Description : main function implementation for Unit Test Framework
// ***************************************************************************

#include <boost/test/unit_test.hpp>   // for unit_test framework
#include <boost/test/unit_test_result.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>
#include <boost/test/detail/unit_test_monitor.hpp>

// BOOST
#include <boost/scoped_ptr.hpp>

// STL
#include <string>
#include <iostream>                   // for cout, cerr

extern boost::unit_test_framework::test_suite* init_unit_test_suite( int argc, char* argv[] );  // prototype for user's test suite init function

// ************************************************************************** //
// **************                 unit test main               ************** //
// ************************************************************************** //

int
main( int argc, char* argv[] )
{
    using namespace boost::unit_test_framework;

    std::string         loglevel;
    bool                no_result_code;
    bool                print_build_info;
    result_report_level report_level;

    // 1. set the log level
    unit_test_log::instance().set_log_threshold_level_by_name(
        retrieve_framework_parameter( LOGLEVEL, &argc, argv ).data() );

    // 2. set the result code and build info flags
    no_result_code    = retrieve_framework_parameter( NO_RESULT_CODE, &argc, argv ) == "no";
    print_build_info  = retrieve_framework_parameter( BUILD_INFO, &argc, argv ) == "yes";

    // 3. set the report type
    std::string report_type_to_set = retrieve_framework_parameter( RESULT_REPORT, &argc, argv );

    if( report_type_to_set == report_level_names[CONFIRMATION_REPORT] ) {
        report_level = CONFIRMATION_REPORT;
    }
    else if( report_type_to_set == report_level_names[SHORT_REPORT] ) {
        report_level = SHORT_REPORT;
    }
    else if( report_type_to_set == report_level_names[DETAILED_REPORT] ) {
        report_level = DETAILED_REPORT;
    }
    else if( report_type_to_set == report_level_names[NO_REPORT] ) {
        report_level = NO_REPORT;
    }
    else if( report_type_to_set == "" ) {
        report_level = CONFIRMATION_REPORT;
    }
    else {
        std::cerr << "*** Unrecognized report level" << std::endl;
        return -999;
    }

    // 4. set catch_system_error switch
    detail::unit_test_monitor::catch_system_errors( retrieve_framework_parameter( CATCH_SYS_ERRORS, &argc, argv ) != "no" );

    // init master unit test suite
    boost::scoped_ptr<test_suite> test( init_unit_test_suite( argc, argv ) );
    if( !test ) {
        std::cerr << "*** Fail to initialize test suite" << std::endl;
        return -999;
    }

    // start testing
    unit_test_log::instance().start( test->size(), print_build_info );
    test->run();

    // odd case: we have 1 test case in 1 test suite
    if( test->size() == 1 )
        unit_test_log::instance() << report_progress();

    // report result
    switch( report_level ) {
    case CONFIRMATION_REPORT:
        unit_test_result::instance().confirmation_report( std::cout );
        break;
    case SHORT_REPORT:
        unit_test_result::instance().short_report( std::cout );
        break;
    case DETAILED_REPORT:
        unit_test_result::instance().detailed_report( std::cout );
        break;
    case NO_REPORT:
    default:
        break;
    }

    // return code
    return no_result_code ? boost::exit_success : unit_test_result::instance().result_code();
}

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.8  2002/12/08 18:11:39  rogeeff
//  catch system errors switch added
//  switch back to scoped_ptr instead of raw test_suite pointer
//
//  Revision 1.7  2002/11/02 20:04:42  rogeeff
//  release 1.29.0 merged into the main trank
//

// ***************************************************************************

// EOF
