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
//  Description : simple implementation for Unit Test Framework parameter
//  handling routines. May be rewritten in future to use some kind of
//  command-line arguments parsing facility and environment variable handling
//  facility
// ***************************************************************************

#ifndef BOOST_TEST_UNIT_TEST_PARAMETERS_IPP_012205GER
#define BOOST_TEST_UNIT_TEST_PARAMETERS_IPP_012205GER

// Boost.Test
#include <boost/test/detail/unit_test_parameters.hpp>
#include <boost/test/utils/basic_cstring/basic_cstring.hpp>
#include <boost/test/utils/basic_cstring/compare.hpp>
#include <boost/test/utils/fixed_mapping.hpp>

//BOOST
#include <boost/config.hpp>           // for broken compiler workarounds
#include <map>
#include <cstdlib>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

# ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::getenv; using ::strncmp; using ::strcmp; }
# endif

namespace boost {

namespace unit_test {

namespace {

// framework parameters and there corresponding command-line arguments
literal_string LOG_LEVEL         = "BOOST_TEST_LOG_LEVEL";
literal_string NO_RESULT_CODE    = "BOOST_TEST_RESULT_CODE";
literal_string REPORT_LEVEL      = "BOOST_TEST_REPORT_LEVEL";
literal_string TESTS_TO_RUN      = "BOOST_TESTS_TO_RUN";
literal_string SAVE_TEST_PATTERN = "BOOST_TEST_SAVE_PATTERN";
literal_string BUILD_INFO        = "BOOST_TEST_BUILD_INFO";
literal_string SHOW_PROGRESS     = "BOOST_TEST_SHOW_PROGRESS";
literal_string CATCH_SYS_ERRORS  = "BOOST_TEST_CATCH_SYSTEM_ERRORS";
literal_string REPORT_FORMAT     = "BOOST_TEST_REPORT_FORMAT";
literal_string LOG_FORMAT        = "BOOST_TEST_LOG_FORMAT";
literal_string OUTPUT_FORMAT     = "BOOST_TEST_OUTPUT_FORMAT";
literal_string DETECT_MEM_LEAKS  = "BOOST_TEST_DETECT_MEMORY_LEAKS";

unit_test::log_level     s_log_level;
bool                     s_no_result_code;
unit_test::report_level  s_report_level;
const_string             s_tests_to_run;
bool                     s_save_pattern;
bool                     s_show_build_info;
bool                     s_show_progress;
bool                     s_catch_sys_errors;
output_format            s_report_format;
output_format            s_log_format;
bool                     s_detect_mem_leaks;

// ************************************************************************** //
// **************                 runtime_config               ************** //
// ************************************************************************** //

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
        SHOW_PROGRESS     , "--show_progress",
        CATCH_SYS_ERRORS  , "--catch_system_errors",
        REPORT_FORMAT     , "--report_format",
        LOG_FORMAT        , "--log_format",
        OUTPUT_FORMAT     , "--output_format",
        DETECT_MEM_LEAKS  , "--detect_memory_leaks",

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

} // local namespace

//____________________________________________________________________________//

namespace runtime_config {

void
init( int* argc, char** argv )
{
    fixed_mapping<const_string,unit_test::log_level,case_ins_less<char const> > log_level_name(
        "all"           , log_successful_tests,
        "success"       , log_successful_tests,
        "test_suite"    , log_test_suites,
        "message"       , log_messages,
        "warning"       , log_warnings,
        "error"         , log_all_errors,
        "cpp_exception" , log_cpp_exception_errors,
        "system_error"  , log_system_errors,
        "fatal_error"   , log_fatal_errors,
        "nothing"       , log_nothing,

        invalid_log_level
    );

    fixed_mapping<const_string,unit_test::report_level,case_ins_less<char const> > report_level_name (
        "confirm",  CONFIRMATION_REPORT,
        "short",    SHORT_REPORT,
        "detailed", DETAILED_REPORT,
        "no",       NO_REPORT,

        INV_REPORT_LEVEL
    );

    fixed_mapping<const_string,output_format,case_ins_less<char const> > output_format_name (
        "HRF", CLF,
        "CLF", CLF,
        "XML", XML,

        CLF
    );

    s_no_result_code    = retrieve_framework_parameter( NO_RESULT_CODE, argc, argv ) == "no";
    s_save_pattern      = retrieve_framework_parameter( SAVE_TEST_PATTERN, argc, argv ) == "yes";
    s_show_build_info   = retrieve_framework_parameter( BUILD_INFO, argc, argv ) == "yes";
    s_show_progress     = retrieve_framework_parameter( SHOW_PROGRESS, argc, argv ) == "yes";
    s_catch_sys_errors  = retrieve_framework_parameter( CATCH_SYS_ERRORS, argc, argv ) != "no";
    s_tests_to_run      = retrieve_framework_parameter( TESTS_TO_RUN, argc, argv );

    s_log_level         = log_level_name[retrieve_framework_parameter( LOG_LEVEL, argc, argv )];
    s_report_level      = report_level_name[retrieve_framework_parameter( REPORT_LEVEL, argc, argv )];

    s_report_format     = output_format_name[retrieve_framework_parameter( REPORT_FORMAT, argc, argv )];
    s_log_format        = output_format_name[retrieve_framework_parameter( LOG_FORMAT, argc, argv )];

    const_string output_format = retrieve_framework_parameter( OUTPUT_FORMAT, argc, argv );
    if( !output_format.is_empty() ) {
        s_report_format     = output_format_name[output_format];
        s_log_format        = output_format_name[output_format];
    }

    s_detect_mem_leaks  = retrieve_framework_parameter( DETECT_MEM_LEAKS, argc, argv ) != "no";
}

//____________________________________________________________________________//

unit_test::log_level
log_level()
{
    return s_log_level;
}

//____________________________________________________________________________//

bool
no_result_code()
{
    return s_no_result_code;
}

//____________________________________________________________________________//

unit_test::report_level
report_level()
{
    return s_report_level;
}

//____________________________________________________________________________//

const_string
test_to_run()
{
    return s_tests_to_run;
}

//____________________________________________________________________________//

bool
save_pattern()
{
    return s_save_pattern;
}

//____________________________________________________________________________//

bool
show_progress()
{
    return s_show_progress;
}

//____________________________________________________________________________//

bool
show_build_info()
{
    return s_show_build_info;
}

//____________________________________________________________________________//

bool
catch_sys_errors()
{
    return s_catch_sys_errors;
}

//____________________________________________________________________________//

output_format
report_format()
{
    return s_report_format;
}

//____________________________________________________________________________//

output_format
log_format()
{
    return s_log_format;
}

//____________________________________________________________________________//

bool
detect_memory_leaks()
{
    return s_detect_mem_leaks;
}

//____________________________________________________________________________//

} // namespace runtime_config

} // namespace unit_test

} // namespace boost

//____________________________________________________________________________//

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.4  2005/02/20 08:27:07  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
// ***************************************************************************

#endif // BOOST_TEST_UNIT_TEST_PARAMETERS_IPP_012205GER
