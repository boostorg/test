//  (C) Copyright Gennadiy Rozental 2001-2003.
//  See accompanying license for terms and conditions of use.

//  See http://www.boost.org for updates, documentation, and revision history.
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

//BOOST
#include <boost/config.hpp>           // for broken compiler workarounds
// for strcmp etc:
#include <cstring>
#include <cstdlib>

# ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::getenv; using ::strncmp; using ::strcmp; }
# endif

namespace boost {

namespace unit_test_framework {

const struct parameter_names {
    c_string_literal env_name;
    c_string_literal cla_name;
} parameter_cla_names [] = {
    { LOG_LEVEL         , "--log_level" },
    { NO_RESULT_CODE    , "--result_code" },
    { REPORT_LEVEL      , "--report_level" },
    { TESTS_TO_RUN      , "--run_test" },
    { SAVE_TEST_PATTERN , "--save_pattern" },
    { BUILD_INFO        , "--build_info" },
    { CATCH_SYS_ERRORS  , "--catch_system_errors" },
    { REPORT_FORMAT     , "--report_format" },
    { LOG_FORMAT        , "--log_format" },
    { OUTPUT_FORMAT     , "--output_format" },
    { c_string_literal(), c_string_literal() }
    
} ;

std::string
retrieve_framework_parameter( c_string_literal parameter_name, int* argc, char** argv )
{
    // first try to find parameter among command line arguments if present
    if( argc ) {
        // locate corresponding cla name
        parameter_names const* curr = parameter_cla_names;
        while( curr->env_name && std::strcmp( curr->env_name, parameter_name ) != 0 )
            curr++;

        if( curr->env_name ) {
            std::string parameter_cla_name = curr->cla_name;
            parameter_cla_name += '=';
            
            for( int i = 1; i < *argc; ++i ) {
                if( std::strncmp( parameter_cla_name.c_str(), argv[i], parameter_cla_name.length() ) == 0 ) {
                    std::string result = argv[i] + parameter_cla_name.length();
                    
                    for( int j = i; j < *argc; ++j ) {
                        argv[j] = argv[j+1];
                    }
                    --(*argc);
                    
                    return result;
                }
            }
        }
    }

    c_string_literal env_var_value = std::getenv( parameter_name );
    return  env_var_value ? env_var_value : "";
}

//____________________________________________________________________________//

} // namespace unit_test_framework

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.9  2003/10/27 07:13:32  rogeeff
//  licence update
//
//  Revision 1.8  2003/06/09 09:17:11  rogeeff
//  1.30.beta1
//

// ***************************************************************************

// EOF

