//  (C) Copyright Gennadiy Rozental 2001.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org for most recent version including documentation.

#include <boost/test/unit_test.hpp>   // for unit_test framework
#include <boost/test/unit_test_result.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/detail/unit_test_config.hpp>

// BOOST
#include <boost/scoped_ptr.hpp>

// STL
#include <string>
#include <iostream>                   // for cout, cerr

using namespace boost::unit_test_framework;

extern test_suite* init_unit_test_suite( int argc, char* argv[] );  // prototype for user's test suite init function

// ************************************************************************** //
// **************                 unit test main               ************** //
// ************************************************************************** //

int
main( int argc, char* argv[] )
{
    std::string         loglevel;
    bool                no_result_code;
    result_report_type  report_type;

    // 1. set the log level
    unit_test_log::instance().set_log_threshold_level_by_name(
        retrieve_framework_parameter( LOGLEVEL, &argc, argv ).data() );

    // 2. set the result code flag
    no_result_code = retrieve_framework_parameter( NO_RESULT_CODE, &argc, argv ) == "no";

    // 3. set the report type
    std::string report_type_to_set = retrieve_framework_parameter( RESULT_REPORT, &argc, argv );

    if( report_type_to_set == report_type_names[CONFIRMATION_REPORT] ) {
        report_type = CONFIRMATION_REPORT;
    }
    else if( report_type_to_set == report_type_names[SHORT_REPORT] ) {
        report_type = SHORT_REPORT;
    }
    else if( report_type_to_set == report_type_names[DETAILED_REPORT] ) {
        report_type = DETAILED_REPORT;
    }
    else if( report_type_to_set == report_type_names[NO_REPORT] ) {
        report_type = NO_REPORT;
    }
    else
        report_type = CONFIRMATION_REPORT;

    // init master unit test suite
    test_suite* test = init_unit_test_suite( argc, argv );
    if( test == NULL ) {
        std::cerr << "*** Fail to initialize test suite" << std::endl;
        return -999;
    }

    // start testing
    unit_test_log::instance().start( test->size() );
    test->run();

    // odd case: we have 1 test case in 1 test suite
    if( test->size() == 1 )
        unit_test_log::instance() << report_progress();

    // report result
    switch( report_type ) {
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

    delete test;
    
    // return code
    return no_result_code ? 0 : unit_test_result::instance().result_code();
}

//  Revision History
//   5 Oct 01  Initial version (Gennadiy Rozental)

// EOF
