//  (C) Copyright Gennadiy Rozental 2001-2003.
//  Use, modification, and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : simple implementation for Unit Test Framework parameter 
//  handling routines. May be rewritten in future to use some kind of 
//  command-line arguments parsing facility and environment variable handling 
//  facility
// ***************************************************************************

// Boost.Test
#include <boost/test/detail/unit_test_parameters.hpp>
#include <boost/test/detail/basic_cstring/compare.hpp>

//BOOST
#include <boost/config.hpp>           // for broken compiler workarounds
#include <map>

# ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::getenv; using ::strncmp; using ::strcmp; }
# endif

namespace boost {
    
namespace unit_test {

struct parameter_names_map : std::map<const_string,const_string>
{
    parameter_names_map() {
        insert( std::make_pair( LOG_LEVEL         , BOOST_TEST_STRING_LITERAL( "--log_level" ) ) );
        insert( std::make_pair( NO_RESULT_CODE    , BOOST_TEST_STRING_LITERAL( "--result_code" ) ) );
        insert( std::make_pair( REPORT_LEVEL      , BOOST_TEST_STRING_LITERAL( "--report_level" ) ) );
        insert( std::make_pair( TESTS_TO_RUN      , BOOST_TEST_STRING_LITERAL( "--run_test" ) ) );
        insert( std::make_pair( SAVE_TEST_PATTERN , BOOST_TEST_STRING_LITERAL( "--save_pattern" ) ) );
        insert( std::make_pair( BUILD_INFO        , BOOST_TEST_STRING_LITERAL( "--build_info" ) ) );
        insert( std::make_pair( CATCH_SYS_ERRORS  , BOOST_TEST_STRING_LITERAL( "--catch_system_errors" ) ) );
        insert( std::make_pair( REPORT_FORMAT     , BOOST_TEST_STRING_LITERAL( "--report_format" ) ) );
        insert( std::make_pair( LOG_FORMAT        , BOOST_TEST_STRING_LITERAL( "--log_format" ) ) );
        insert( std::make_pair( OUTPUT_FORMAT     , BOOST_TEST_STRING_LITERAL( "--output_format" ) ) );
    }
};

static parameter_names_map parameter_names;

const_string
retrieve_framework_parameter( const_string parameter_name, int* argc, char** argv )
{
    // first try to find parameter among command line arguments if present
    if( argc ) {
        // locate corresponding cla name
        parameter_names_map::const_iterator it = parameter_names.find( parameter_name );

        if( it != parameter_names.end() ) {
            for( int i = 1; i < *argc; ++i ) {
                if( it->second == const_string( argv[i], it->second.size() ) && 
                    argv[i][it->second.size()] == '=' ) {
                    const_string result = argv[i] + it->second.size() + 1;
                    
                    for( int j = i; j < *argc; ++j ) {
                        argv[j] = argv[j+1];
                    }
                    --(*argc);
                    
                    return result;
                }
            }
        }
    }

    return std::getenv( parameter_name.begin() );
}

//____________________________________________________________________________//

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.12  2004/05/11 11:05:04  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//  namespace names shortened
//
//  Revision 1.11  2003/12/01 00:42:37  rogeeff
//  prerelease cleaning
//

// ***************************************************************************

// EOF

