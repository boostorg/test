//  main function implementation for Program Executon Monitor

//  Copyright Gennadiy Rozental 2001-2003.
//  Copyright Beman Dawes 1995-2001.
//  See accompanying license for terms and conditions of use.

//  See http://www.boost.org/libs/test for documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : main function implementation for Program Executon Monitor
// ***************************************************************************

// Boost.Test
#include <boost/test/execution_monitor.hpp>
#include <boost/test/detail/unit_test_config.hpp>

// BOOST
#include <boost/cstdlib.hpp>
#include <boost/config.hpp>

// STL
#include <iostream>
#include <cstring>

#ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::getenv; using ::strcmp; }
#endif

int cpp_main( int argc, char* argv[] );  // prototype for user's cpp_main()

namespace
{

class cpp_main_caller : public boost::execution_monitor {
public:
    cpp_main_caller( int argc, char** argv )
    : m_argc( argc ), m_argv( argv )    {}
    
    int         function()              { return cpp_main( m_argc, m_argv ); }
    
private:
    int         m_argc;
    char**      m_argv;
};

}

// ************************************************************************** //
// **************                   cpp main                   ************** //
// ************************************************************************** //

int main( int argc, char* argv[] )
{
    cpp_main_caller caller( argc, argv );
    
    int result;

    boost::unit_test_framework::c_string_literal p( std::getenv( "BOOST_TEST_CATCH_SYSTEM_ERRORS" ) );
    bool catch_system_errors = !p || (std::strcmp( p, "no" ) != 0);
        
    try {
        result = caller.execute( catch_system_errors );
        
        if( result == 0 )
            result = boost::exit_success;
        else if( result != boost::exit_success ) {
            std::cout << "\n**** error return code: " << result << std::endl;
            result = boost::exit_failure;
        }
    }
    catch( boost::execution_exception const& exex ) {
        std::cout << "\n**** exception(" << exex.code() << "): " << exex.what() << std::endl;
        result = boost::exit_exception_failure;
    }
    
    if( result != boost::exit_success ) {
        std::cerr << "******** errors detected; see standard output for details ********" << std::endl;
    }
    else {
        //  Some prefer a confirming message when all is well, while others don't
        //  like the clutter.  Use an environment variable to avoid command
        //  line argument modifications; for use in production programs
        //  that's a no-no in some organizations.
        boost::unit_test_framework::c_string_literal p( std::getenv( "BOOST_PRG_MON_CONFIRM" ) );
        if( !p || std::strcmp( p, "no" ) != 0 ) { 
            std::cerr << std::flush << "no errors detected" << std::endl; 
        }
    }

    return result;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.10  2003/09/14 12:42:22  beman_dawes
//  Change to new license (with Gennadiy's permission)
//
//  Revision 1.9  2003/07/11 16:26:21  jmaurer
//  need <cstring> before being able to use std::strcmp etc.
//
//  Revision 1.8  2003/06/09 09:12:26  rogeeff
//  straiten return code logic for Program Execution Monitor
//

// ***************************************************************************

// EOF
