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
//  Description : simple implementation for Unit Test Framework parameter 
//  handling routines. May be rewritten in future to use some kind of 
//  command-line arguments parsing facility and environment variable handling 
//  facility
// ***************************************************************************

#ifndef BOOST_UNIT_TEST_PARAMETERS_IPP_012205GER
#define BOOST_UNIT_TEST_PARAMETERS_IPP_012205GER

// Boost.Test
#include <boost/test/detail/unit_test_parameters.hpp>
#include <boost/test/utils/basic_cstring/compare.hpp>
#include <boost/test/utils/fixed_mapping.hpp>

//BOOST
#include <boost/config.hpp>           // for broken compiler workarounds
#include <map>
#include <cstdlib>

# ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::getenv; using ::strncmp; using ::strcmp; }
# endif

namespace boost {
    
namespace unit_test {

const_string
retrieve_framework_parameter( const_string parameter_name, int* argc, char** argv )
{
    static fixed_mapping<const_string,const_string> parameter_2_cla_name_map(
        LOG_LEVEL         , "--log_level",
        NO_RESULT_CODE    , "--result_code",
        REPORT_LEVEL      , "--report_level",
        TESTS_TO_RUN      , "--run_test",
        SAVE_TEST_PATTERN , "--save_pattern",
        BUILD_INFO        , "--build_info",
        CATCH_SYS_ERRORS  , "--catch_system_errors",
        REPORT_FORMAT     , "--report_format",
        LOG_FORMAT        , "--log_format",
        OUTPUT_FORMAT     , "--output_format",

        ""
    );

    // first try to find parameter among command line arguments if present
    if( argc ) {
        // locate corresponding cla name
        const_string cla_name = parameter_2_cla_name_map[parameter_name];

        if( !cla_name.is_empty() ) {
            for( int i = 1; i < *argc; ++i ) {
                if( cla_name == const_string( argv[i], cla_name.size() ) && 
                    argv[i][cla_name.size()] == '=' ) {
                    const_string result = argv[i] + cla_name.size() + 1;
                    
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
//  Revision 1.1  2005/01/22 19:22:13  rogeeff
//  implementation moved into headers section to eliminate dependency of included/minimal component on src directory
//
//  Revision 1.15  2004/05/21 06:26:10  rogeeff
//  licence update
//
//  Revision 1.14  2004/05/18 13:34:15  dgregor
//  Needed to include <cstdlib> to get std::getenv.
//
//  Revision 1.13  2004/05/13 09:04:43  rogeeff
//  added fixed_mapping
//
//  Revision 1.12  2004/05/11 11:05:04  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//  namespace names shortened
//
//  Revision 1.11  2003/12/01 00:42:37  rogeeff
//  prerelease cleaning
//

// ***************************************************************************

#endif // BOOST_UNIT_TEST_PARAMETERS_IPP_012205GER
