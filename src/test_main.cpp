//  (C) Copyright Gennadiy Rozental 2001-2002.
//  (C) Copyright Beman Dawes 1995-2001. 
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
//  Description : implements main function for Test Execution Monitor. 
// ***************************************************************************

// Boost.Test
#include <boost/test/unit_test.hpp>         // for unit test framework
#include <boost/test/unit_test_result.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>

// BOOST
#include <boost/scoped_ptr.hpp>

// STL
#include <iostream>                         // for cout, cerr

int test_main( int argc, char* argv[] );    // prototype for user's test_main()

namespace {
    int      argc_;
    char**   argv_;
    int      test_main_result;
    
    void call_test_main()
    {
        test_main_result = test_main( argc_, argv_ );
        
        // translate a test_main non-success return into a test error
        BOOST_CHECK( test_main_result == 0 || test_main_result == boost::exit_success );
    }
}

// ************************************************************************** //
// **************                   test main                  ************** //
// ************************************************************************** //

int main( int argc, char* argv[] ) {
    using namespace boost::unit_test_framework;

    std::string         loglevel;
    bool                no_result_code;
    result_report_level report_level;

    // 1. set the log level
    unit_test_log::instance().set_log_threshold_level_by_name( retrieve_framework_parameter( LOGLEVEL, &argc, argv ).data() );

    // 2. set the result code flag
    no_result_code = retrieve_framework_parameter( NO_RESULT_CODE, &argc, argv ) == "no";

    // 3. set the report level
    std::string report_level_to_set = retrieve_framework_parameter( RESULT_REPORT, &argc, argv );

    report_level = report_level_to_set == report_level_names[NO_REPORT] ? NO_REPORT : CONFIRMATION_REPORT;

    unit_test_log::instance().set_log_threshold_level_by_name( loglevel.data() );

    //  set up the test   
    argc_ = argc;
    argv_ = argv;
    boost::scoped_ptr<test_case> test_main_tc( BOOST_TEST_CASE( &call_test_main ) );

    // start testing
    test_main_tc->run();
    
    // report result
    if( report_level == CONFIRMATION_REPORT )
        unit_test_result::instance().confirmation_report( std::cout );

    // return code
    return no_result_code 
            ? boost::exit_success
            : ( test_main_result != 0 && test_main_result != boost::exit_success 
                   ? test_main_result 
                   : unit_test_result::instance().result_code() 
              );
}

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.9  2002/11/02 20:04:41  rogeeff
//  release 1.29.0 merged into the main trank
//

// ***************************************************************************

// EOF
