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

// ************************************************************************** //
// **************                 unit test main               ************** //
// ************************************************************************** //

#ifdef BOOST_TEST_DYN_LINK

namespace boost {

namespace unit_test {

int BOOST_TEST_DECL
dll_main( bool (*init_unit_test_func)(), int argc, char* argv[] )

#else

int BOOST_TEST_CALL_DECL
main( int argc, char* argv[] )

#endif
{
    using namespace boost::unit_test;
   
    try {
        framework::init( argc, argv );

#ifdef BOOST_TEST_DYN_LINK
    if( !(*init_unit_test_func)() )
        throw std::logic_error( "Test failed to initialize" );
#endif

        framework::run();

        results_reporter::make_report();

        return runtime_config::no_result_code() 
                    ? boost::exit_success 
                    : results_collector.results( framework::master_test_suite().p_id ).result_code();
    }
    catch( std::logic_error const& ex ) {
        std::cerr << "Boost.Test framework error: " << ex.what() << std::endl;
        
        return boost::exit_exception_failure;
    }
    catch( ... ) {
        std::cerr << "Boost.Test internal framework error: unknown reason" << std::endl;
        
        return boost::exit_exception_failure;
    }
}

#ifdef BOOST_TEST_DYN_LINK

} // namespace unit_test

} // namespace boost

#endif

//____________________________________________________________________________//

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.7  2005/12/14 05:35:57  rogeeff
//  DLL support implemented
//  Alternative init API introduced
//
//  Revision 1.6  2005/02/20 08:27:07  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
// ***************************************************************************

#endif // BOOST_TEST_UNIT_TEST_MAIN_IPP_012205GER
