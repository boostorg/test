//  (C) Copyright Gennadiy Rozental 2001-2003.
//  (C) Copyright Beman Dawes 1995-2001. 
//  See accompanying license for terms and conditions of use.

//  See http://www.boost.org/libs/test for documentation.
//
//  File        : $RCSfile$
//
//  Version     : $$Revision$
//
//  Description : implements main function for Test Execution Monitor. 
// ***************************************************************************

// Boost.Test
#include <boost/test/unit_test.hpp>         // for unit test framework
#include <boost/test/unit_test_result.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>
#include <boost/test/detail/unit_test_monitor.hpp>

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

    // set the log level
    unit_test_log::instance().set_log_threshold_level_by_name( retrieve_framework_parameter( LOG_LEVEL, &argc, argv ) );

    // set the report level
    std::string reportlevel = retrieve_framework_parameter( REPORT_LEVEL, &argc, argv );

    // set the log/report format
    std::string output_format = retrieve_framework_parameter( OUTPUT_FORMAT, &argc, argv );
    
    if( output_format.empty() ) {
        unit_test_log::instance().set_log_format( retrieve_framework_parameter( LOG_FORMAT, &argc, argv ) );
        unit_test_result::set_report_format( retrieve_framework_parameter( REPORT_FORMAT, &argc, argv ) );
    }
    else {
        unit_test_log::instance().set_log_format( output_format );
        unit_test_result::set_report_format( output_format );
    }

    // set the result code flag
    bool no_result_code = retrieve_framework_parameter( NO_RESULT_CODE, &argc, argv ) == "no";

    // set catch_system_error switch
    detail::unit_test_monitor::catch_system_errors( retrieve_framework_parameter( CATCH_SYS_ERRORS, &argc, argv ) != "no" );

    //  set up the test   
    argc_ = argc;
    argv_ = argv;
    boost::scoped_ptr<test_case> test_main_tc( BOOST_TEST_CASE( &call_test_main ) );

    // start testing
    unit_test_log::instance().start( retrieve_framework_parameter( BUILD_INFO, &argc, argv ) == "yes" );
    test_main_tc->run();
    unit_test_log::instance().finish( 1 );
    
    // report results
    unit_test_result::instance().report( reportlevel, std::cout );

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
//  Revision 1.13  2003/10/27 07:13:32  rogeeff
//  licence update
//
//  Revision 1.12  2003/09/14 12:42:22  beman_dawes
//  Change to new license (with Gennadiy's permission)
//
//  Revision 1.11  2003/06/09 09:10:50  rogeeff
//  added support for catch_system_error in Test Execution Monitor
//

// ***************************************************************************

// EOF
