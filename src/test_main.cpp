//  (C) Copyright Gennadiy Rozental 2001-2004.
//  (C) Copyright Beman Dawes 1995-2001. 
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
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
    using namespace boost::unit_test;

    // set the log level
    unit_test_log.set_threshold_level_by_name( retrieve_framework_parameter( LOG_LEVEL, &argc, argv ) );

    // set the report level
    const_string reportlevel = retrieve_framework_parameter( REPORT_LEVEL, &argc, argv );

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

    // set the result code flag
    bool no_result_code = retrieve_framework_parameter( NO_RESULT_CODE, &argc, argv ) == "no";

    // set catch_system_error switch
    ut_detail::unit_test_monitor::catch_system_errors( retrieve_framework_parameter( CATCH_SYS_ERRORS, &argc, argv ) != "no" );

    //  set up the test   
    argc_ = argc;
    argv_ = argv;
    boost::scoped_ptr<test_case> test_main_tc( BOOST_TEST_CASE( &call_test_main ) );

    // start testing
    unit_test_log.start( retrieve_framework_parameter( BUILD_INFO, &argc, argv ) == "yes" );
    test_main_tc->run();
    unit_test_log.finish( 1 );
    
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
//  Revision 1.19  2005/01/18 08:30:08  rogeeff
//  unit_test_log rework:
//     eliminated need for ::instance()
//     eliminated need for << end and ...END macro
//     straitend interface between log and formatters
//     change compiler like formatter name
//     minimized unit_test_log interface and reworked to use explicit calls
//
//  Revision 1.18  2004/06/07 07:34:22  rogeeff
//  detail namespace renamed
//
//  Revision 1.17  2004/05/21 06:26:09  rogeeff
//  licence update
//
//  Revision 1.16  2004/05/11 11:04:44  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//  namespace names shortened
//
//  Revision 1.15  2003/12/01 00:42:37  rogeeff
//  prerelease cleaning
//

// ***************************************************************************

// EOF
