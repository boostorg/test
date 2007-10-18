//  (C) Copyright Gennadiy Rozental 2001-2005.
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

#ifndef BOOST_TEST_UNIT_TEST_MAIN_IPP_012205GER
#define BOOST_TEST_UNIT_TEST_MAIN_IPP_012205GER

// Boost.Test
#include <boost/test/framework.hpp>
#include <boost/test/results_collector.hpp>
#include <boost/test/unit_test_suite_impl.hpp>
#include <boost/test/results_reporter.hpp>

#include <boost/test/detail/unit_test_parameters.hpp>

// Boost
#include <boost/cstdlib.hpp>

// STL
#include <stdexcept>
#include <iostream>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************                  unit_test_main              ************** //
// ************************************************************************** //

int BOOST_TEST_DECL
unit_test_main( init_unit_test_func init_func, int argc, char* argv[] )
{
    try {
        framework::init( init_func, argc, argv );

// !! ??       if( !runtime_config.test_to_run().is_empty() ) {
//
//        }

        framework::run();

        results_reporter::make_report();

        return runtime_config::no_result_code() 
                    ? boost::exit_success 
                    : results_collector.results( framework::master_test_suite().p_id ).result_code();
    }
    catch( framework::internal_error const& ex ) {
        results_reporter::get_stream() << "Boost.Test framework internal error: " << ex.what() << std::endl;
        
        return boost::exit_exception_failure;
    }
    catch( framework::setup_error const& ex ) {
        results_reporter::get_stream() << "Test setup error: " << ex.what() << std::endl;
        
        return boost::exit_exception_failure;
    }
    catch( ... ) {
        results_reporter::get_stream() << "Boost.Test framework internal error: unknown reason" << std::endl;
        
        return boost::exit_exception_failure;
    }
}

} // namespace unit_test

} // namespace boost

#if !defined(BOOST_TEST_DYN_LINK) && !defined(BOOST_TEST_NO_MAIN)

// ************************************************************************** //
// **************        main function for tests using lib     ************** //
// ************************************************************************** //

int BOOST_TEST_CALL_DECL
main( int argc, char* argv[] )
{
    // prototype for user's unit test init function
#ifdef BOOST_TEST_ALTERNATIVE_INIT_API
    extern bool init_unit_test();

    boost::unit_test::init_unit_test_func init_func = &init_unit_test;
#else
    extern ::boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] );

    boost::unit_test::init_unit_test_func init_func = &init_unit_test_suite;
#endif

    return ::boost::unit_test::unit_test_main( init_func, argc, argv );
}

#endif // !BOOST_TEST_DYN_LINK && !BOOST_TEST_NO_MAIN

//____________________________________________________________________________//

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.9  2006/03/19 11:45:26  rogeeff
//  main function renamed for consistancy
//
//  Revision 1.8  2006/03/19 07:27:52  rogeeff
//  streamline test setup error message
//
//  Revision 1.7  2005/12/14 05:35:57  rogeeff
//  DLL support implemented
//  Alternative init API introduced
//
//  Revision 1.6  2005/02/20 08:27:07  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
// ***************************************************************************

#endif // BOOST_TEST_UNIT_TEST_MAIN_IPP_012205GER
