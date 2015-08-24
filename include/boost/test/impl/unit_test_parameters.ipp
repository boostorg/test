//  (C) Copyright Gennadiy Rozental 2001-2014.
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
#include <boost/test/unit_test_parameters.hpp>
#include <boost/test/utils/basic_cstring/basic_cstring.hpp>
#include <boost/test/utils/basic_cstring/compare.hpp>
#include <boost/test/utils/basic_cstring/io.hpp>
#include <boost/test/utils/fixed_mapping.hpp>
#include <boost/test/debug.hpp>
#include <boost/test/framework.hpp>

#include <boost/test/detail/throw_exception.hpp>

// Boost.Runtime.Param
#include <boost/test/utils/runtime/parameter.hpp>
#include <boost/test/utils/runtime/argument.hpp>
#include <boost/test/utils/runtime/finalize.hpp>

#include <boost/test/utils/runtime/cla/parser.hpp>

#include <boost/test/utils/runtime/env/fetch.hpp>

namespace rt = boost::runtime;

#ifndef UNDER_CE
// !!!! #include <boost/test/utils/runtime/env/variable.hpp>

// !!!! namespace env = rt::env;
#endif

// Boost
#include <boost/config.hpp>
#include <boost/test/detail/suppress_warnings.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/detail/enable_warnings.hpp>
#include <boost/optional.hpp>

// STL
#include <cstdlib>
#include <iostream>
#include <fstream>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

# ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::getenv; using ::strncmp; using ::strcmp; }
# endif

namespace boost {
namespace unit_test {

// ************************************************************************** //
// **************    input operations for unit_test's enums    ************** //
// ************************************************************************** //

std::istream&
operator>>( std::istream& in, unit_test::log_level& ll )
{
    static fixed_mapping<const_string,log_level,case_ins_less<char const> > log_level_name(
        "all"           , log_successful_tests,
        "success"       , log_successful_tests,
        "test_suite"    , log_test_units,
        "unit_scope"    , log_test_units,
        "message"       , log_messages,
        "warning"       , log_warnings,
        "error"         , log_all_errors,
        "cpp_exception" , log_cpp_exception_errors,
        "system_error"  , log_system_errors,
        "fatal_error"   , log_fatal_errors,
        "nothing"       , log_nothing,

        invalid_log_level
        );

    std::string val;
    in >> val;

    ll = log_level_name[val];
    BOOST_TEST_SETUP_ASSERT( ll != invalid_log_level, "invalid log level " + val );

    return in;
}

//____________________________________________________________________________//

std::istream&
operator>>( std::istream& in, unit_test::report_level& rl )
{
    fixed_mapping<const_string,unit_test::report_level,case_ins_less<char const> > report_level_name (
        "confirm",  CONFIRMATION_REPORT,
        "short",    SHORT_REPORT,
        "detailed", DETAILED_REPORT,
        "no",       NO_REPORT,

        INV_REPORT_LEVEL
        );

    std::string val;
    in >> val;

    rl = report_level_name[val];
    BOOST_TEST_SETUP_ASSERT( rl != INV_REPORT_LEVEL, "invalid report level " + val );

    return in;
}

//____________________________________________________________________________//

std::istream&
operator>>( std::istream& in, unit_test::output_format& of )
{
    fixed_mapping<const_string,unit_test::output_format,case_ins_less<char const> > output_format_name (
        "HRF", OF_CLF,
        "CLF", OF_CLF,
        "XML", OF_XML,
        "DOT", OF_DOT,

        OF_INVALID
        );

    std::string val;
    in >> val;

    of = output_format_name[val];
    BOOST_TEST_SETUP_ASSERT( of != OF_INVALID, "Invalid output format " + val );

    return in;
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************                 runtime_config               ************** //
// ************************************************************************** //

namespace runtime_config {

namespace {

// UTF parameters 
std::string AUTO_START_DBG    = "auto_start_dbg";
std::string BREAK_EXEC_PATH   = "break_exec_path";
std::string BUILD_INFO        = "build_info";
std::string CATCH_SYS_ERRORS  = "catch_system_errors";
std::string COLOR_OUTPUT      = "color_output";
std::string DETECT_FP_EXCEPT  = "detect_fp_exceptions";
std::string DETECT_MEM_LEAKS  = "detect_memory_leaks";
std::string LIST_CONTENT      = "list_content";
std::string LIST_LABELS       = "list_labels";
std::string LOG_FORMAT        = "log_format";
std::string LOG_LEVEL         = "log_level";
std::string LOG_SINK          = "log_sink";
std::string OUTPUT_FORMAT     = "output_format";
std::string RANDOM_SEED       = "random";
std::string REPORT_FORMAT     = "report_format";
std::string REPORT_LEVEL      = "report_level";
std::string REPORT_SINK       = "report_sink";
std::string RESULT_CODE       = "result_code";
std::string RUN_FILTERS       = "run_test";
std::string SAVE_TEST_PATTERN = "save_pattern";
std::string SHOW_PROGRESS     = "show_progress";
std::string USE_ALT_STACK     = "use_alt_stack";
std::string WAIT_FOR_DEBUGGER = "wait_for_debugger";
std::string HELP              = "help";

//____________________________________________________________________________//

void
register_parameters( rt::parameters_store& store )
{
    rt::option auto_start_dbg( AUTO_START_DBG, (
        rt::description = "Automatically starts debugger if system level error (signal) occurs",
        rt::env_var = "BOOST_TEST_AUTO_START_DBG"
    ));

    auto_start_dbg.add_cla_id( "--", AUTO_START_DBG, "=" );
    auto_start_dbg.add_cla_id( "-", "d", " " );
    store.add( auto_start_dbg );

    rt::parameter<std::string> break_exec_path( BREAK_EXEC_PATH, (
        rt::description = "For the exception safety testing allows to break at specific execution path",
        rt::env_var = "BOOST_TEST_BREAK_EXEC_PATH"
    ));

    break_exec_path.add_cla_id( "--", BREAK_EXEC_PATH, "=" );    
    store.add( break_exec_path );

    rt::option build_info( BUILD_INFO, (
        rt::description = "Shows library build information",
        rt::env_var = "BOOST_TEST_BUILD_INFO"
    ));

    build_info.add_cla_id( "--", BUILD_INFO, "=" );
    build_info.add_cla_id( "-", "i", " " );
    store.add( build_info );


    rt::option catch_sys_errors( CATCH_SYS_ERRORS, (
        rt::description = "Allows to switch between catching and ignoring system errors (signals)",
        rt::env_var = "BOOST_TEST_CATCH_SYSTEM_ERRORS",
        rt::default_value =
#ifdef BOOST_TEST_DEFAULTS_TO_CORE_DUMP
            false
#else
            true
#endif
    ));

    catch_sys_errors.add_cla_id( "--", CATCH_SYS_ERRORS, "=", true );
    catch_sys_errors.add_cla_id( "-", "s", " " );
    store.add( catch_sys_errors );

    rt::option color_output( COLOR_OUTPUT, (
        rt::description = "Enables color output of the framework log and report messages",
        rt::env_var = "BOOST_TEST_COLOR_OUTPUT"
    ));

    color_output.add_cla_id( "--", COLOR_OUTPUT, "=", true );
    color_output.add_cla_id( "-", "x", " " );
    store.add( color_output );

    rt::option detect_fp_except( DETECT_FP_EXCEPT, (
        rt::description = "Allows to switch between catching and ignoring floating point exceptions",
        rt::env_var = "BOOST_TEST_DETECT_FP_EXCEPTIONS"
    ));

    detect_fp_except.add_cla_id( "--", DETECT_FP_EXCEPT, "=", true );
    store.add( detect_fp_except );

    rt::parameter<std::string> detect_mem_leaks( DETECT_MEM_LEAKS, (
        rt::description = "Allows to switch between catching and ignoring memory leaks",
        rt::env_var = "BOOST_TEST_DETECT_MEMORY_LEAK"
    ));
    detect_mem_leaks.add_cla_id( "--", DETECT_MEM_LEAKS, "=" );
    store.add( detect_mem_leaks );

    rt::parameter<unit_test::output_format> list_content( LIST_CONTENT, (
        rt::optional_value,
        rt::description = "Lists the content of test tree - names of all test suites and test cases",
        rt::env_var = "BOOST_TEST_LIST_CONTENT",
        rt::default_value = OF_INVALID,
        rt::optional_value = OF_CLF
    ));
    list_content.add_cla_id( "--", LIST_CONTENT, "=" );    
    store.add( list_content );

    rt::option list_labels( LIST_LABELS, (
        rt::description = "Lists all available labels",
        rt::env_var = "BOOST_TEST_LIST_LABELS"
    ));

    list_labels.add_cla_id( "--", LIST_LABELS, "=" );
    store.add( list_labels );

    rt::parameter<unit_test::output_format> log_format( LOG_FORMAT, (
        rt::description = "Specifies log format",
        rt::env_var = "BOOST_TEST_LOG_FORMAT",
        rt::default_value = OF_CLF
    ));

    log_format.add_cla_id( "--", LOG_FORMAT, "=" );
    log_format.add_cla_id( "-", "f", " " );
    store.add( log_format );

    rt::parameter<unit_test::log_level> log_level( LOG_LEVEL, (
        rt::description = "Specifies log level",
        rt::env_var = "BOOST_TEST_LOG_LEVEL",
        rt::default_value = log_all_errors
    ));

    log_level.add_cla_id( "--", LOG_LEVEL, "=" );
    log_level.add_cla_id( "-", "l", " " );
    store.add( log_level );

    rt::parameter<std::string> log_sink( LOG_SINK, (
        rt::description = "Specifies log sink: stdout(default), stderr or file name",
        rt::env_var = "BOOST_TEST_LOG_SINK"
    ));

    log_sink.add_cla_id( "--", LOG_SINK, "=" );
    log_sink.add_cla_id( "-", "k", " " );
    store.add( log_sink );

    rt::parameter<unit_test::output_format> output_format( OUTPUT_FORMAT, (
        rt::description = "Specifies output format (both log and report)",
        rt::env_var = "BOOST_TEST_OUTPUT_FORMAT"
    ));

    output_format.add_cla_id( "--", OUTPUT_FORMAT, "=" );
    output_format.add_cla_id( "-", "o", " " );
    store.add( output_format );

    rt::parameter<unsigned> random_seed( RANDOM_SEED, (
        rt::optional_value,
        rt::description = "Allows to switch between sequential and random order of test units execution.\n"
                           "Optionally allows to specify concrete seed for random number generator",
        rt::env_var = "BOOST_TEST_RANDOM",
        rt::default_value = 0U,
        rt::optional_value = 1U
    ));

    random_seed.add_cla_id( "--", RANDOM_SEED, "=" );    
    store.add( random_seed );

    rt::parameter<unit_test::output_format> report_format( REPORT_FORMAT, (
        rt::description = "Specifies report format",
        rt::env_var = "BOOST_TEST_REPORT_FORMAT",
        rt::default_value = OF_CLF
    ));

    report_format.add_cla_id( "--", REPORT_FORMAT, "=" );
    report_format.add_cla_id( "-", "m", " " );
    store.add( report_format );

    rt::parameter<unit_test::report_level> report_level( REPORT_LEVEL, (
        rt::description = "Specifies report level",
        rt::env_var = "BOOST_TEST_REPORT_LEVEL",
        rt::default_value = CONFIRMATION_REPORT
    ));

    report_level.add_cla_id( "--", REPORT_LEVEL, "=" );
    report_level.add_cla_id( "-", "r", " " );
    store.add( report_level );

    rt::parameter<std::string> report_sink( REPORT_SINK, (
        rt::description = "Specifies report sink: stderr(default), stdout or file name",
        rt::env_var = "BOOST_TEST_REPORT_SINK"
    ));

    report_sink.add_cla_id( "--", REPORT_SINK, "=" );
    report_sink.add_cla_id( "-", "e", " " );
    store.add( report_sink );

    rt::option result_code( RESULT_CODE, (
        rt::description = "Disables test modules's result code generation",
        rt::env_var = "BOOST_TEST_RESULT_CODE",
        rt::default_value = true
    ));

    result_code.add_cla_id( "--", RESULT_CODE, "=", true );
    result_code.add_cla_id( "-", "c", " " );
    store.add( result_code );

    rt::parameter<std::string,rt::REPEATABLE> tests_to_run( RUN_FILTERS, (
        rt::description = "Filters, which test units to include or exclude from test module execution",
        rt::env_var = "BOOST_TEST_RUN_FILTERS"
    ));

    tests_to_run.add_cla_id( "--", RUN_FILTERS, "=" );    
    tests_to_run.add_cla_id( "-", "t", " " );
    store.add( tests_to_run );

    rt::option save_test_pattern( SAVE_TEST_PATTERN, (
        rt::description = "Allows to switch between saving and matching against test pattern file",
        rt::env_var = "BOOST_TEST_SAVE_PATTERN"
    ));

    save_test_pattern.add_cla_id( "--", SAVE_TEST_PATTERN, "=" );    
    store.add( save_test_pattern );

    rt::option show_progress( SHOW_PROGRESS, (
        rt::description = "Turns on progress display",
        rt::env_var = "BOOST_TEST_SHOW_PROGRESS"
    ));

    show_progress.add_cla_id( "--", SHOW_PROGRESS, "=" );
    show_progress.add_cla_id( "-", "p", " " );
    store.add( show_progress );

    rt::option use_alt_stack( USE_ALT_STACK, (
        rt::description = "Turns on/off usage of an alternative stack for signal handling",
        rt::env_var = "BOOST_TEST_USE_ALT_STACK",
        rt::default_value = true
    ));

    use_alt_stack.add_cla_id( "--", USE_ALT_STACK, "=", true );
    store.add( use_alt_stack );

    rt::option wait_for_debugger( WAIT_FOR_DEBUGGER, (
        rt::description = "Forces test module to wait for button to be pressed before starting test run",
        rt::env_var = "BOOST_TEST_WAIT_FOR_DEBUGGER"
    ));

    wait_for_debugger.add_cla_id( "--", WAIT_FOR_DEBUGGER, "=" );
    wait_for_debugger.add_cla_id( "-", "w", " " );
    store.add( wait_for_debugger );

    rt::parameter<std::string> help( HELP, (
        rt::description = "Help for framework parameters",
        rt::optional_value = std::string()
    ));
    help.add_cla_id( "--", HELP, "=" );
    store.add( help );
}

static rt::arguments_store  s_arguments_store;
static rt::parameters_store s_parameters_store;
static rt::arguments_store  const& s_const_arguments_store = s_arguments_store;

//____________________________________________________________________________//

// !!!!
#if 0
void
disable_use( rt::parameter const&, std::string const& )
{
    BOOST_TEST_SETUP_ASSERT( false, "parameter break_exec_path is disabled in this release" );
}
#endif

//____________________________________________________________________________//

} // local namespace

void
init( int& argc, char** argv )
{
    BOOST_TEST_IMPL_TRY {
        // Initialize parameters list
        if( s_parameters_store.is_empty() )
            register_parameters( s_parameters_store );

        // Clear up arguments store just in case (of multiple init invocations)
        s_arguments_store.clear();

        // Parse CLA they take precedence over  environment
        rt::cla::parser parser( s_parameters_store, (rt::end_of_params = "--", rt::negation_prefix = "no_") );
        parser.parse( argc, argv, s_arguments_store );

        // Try to fetch missing arguments from environment
        rt::env::fetch_absent( s_parameters_store, s_arguments_store );

        // Set arguments to default values if defined and perform all the validations
        rt::finalize_arguments( s_parameters_store, s_arguments_store );

        // Report help if requested
        if( s_const_arguments_store.has( HELP ) ) {
            parser.usage( std::cerr, s_parameters_store, s_const_arguments_store.get<std::string>( HELP ) );
            BOOST_TEST_IMPL_THROW( framework::nothing_to_test() );
        }

        // A bit of business logic: output_format takes precedence over log/report formats
        if( s_arguments_store.has( OUTPUT_FORMAT ) ) {
            unit_test::output_format of = s_arguments_store.get<unit_test::output_format>( OUTPUT_FORMAT );
            s_arguments_store.set( REPORT_FORMAT, of );
            s_arguments_store.set( LOG_FORMAT, of );
        }
    }
    BOOST_TEST_IMPL_CATCH( rt::init_error, ex ) {
        BOOST_TEST_SETUP_ASSERT( false, "Internal Boost.Test initialization error: " + ex.msg );
    }
    BOOST_TEST_IMPL_CATCH( rt::input_error, ex ) {
        // !!!! help/usage?
        // !!!! unidentified arg, 
        // !!!! ambigous arg, 
        BOOST_TEST_SETUP_ASSERT( false, ex.msg );
    }
}

//____________________________________________________________________________//

unit_test::log_level
log_level()
{
    return s_const_arguments_store.get<unit_test::log_level>( LOG_LEVEL );
}

//____________________________________________________________________________//

bool
no_result_code()
{
    return !s_const_arguments_store.get<bool>( RESULT_CODE );
}

//____________________________________________________________________________//

unit_test::report_level
report_level()
{
    return s_const_arguments_store.get<unit_test::report_level>( REPORT_LEVEL );
}

//____________________________________________________________________________//

std::vector<std::string> const&
run_filters()
{
    return s_const_arguments_store.get<std::vector<std::string>>( RUN_FILTERS );
}

//____________________________________________________________________________//

const_string
break_exec_path()
{
    return s_const_arguments_store.get<std::string>( BREAK_EXEC_PATH );
}

//____________________________________________________________________________//

bool
save_pattern()
{
    return s_const_arguments_store.get<bool>( SAVE_TEST_PATTERN );
}

//____________________________________________________________________________//

bool
show_progress()
{
    return s_const_arguments_store.get<bool>( SHOW_PROGRESS );
}

//____________________________________________________________________________//

bool
show_build_info()
{
    return s_const_arguments_store.get<bool>( BUILD_INFO );
}

//____________________________________________________________________________//

unit_test::output_format
list_content()
{
    return s_const_arguments_store.get<unit_test::output_format>( LIST_CONTENT );
}

//____________________________________________________________________________//

bool
list_labels()
{
    return s_const_arguments_store.get<bool>( LIST_LABELS );
}

//____________________________________________________________________________//

bool
catch_sys_errors()
{
    return s_const_arguments_store.get<bool>( CATCH_SYS_ERRORS );
}

//____________________________________________________________________________//

bool
color_output()
{
    return s_const_arguments_store.get<bool>( COLOR_OUTPUT );
}

//____________________________________________________________________________//

bool
auto_start_dbg()
{
    return s_const_arguments_store.get<bool>( AUTO_START_DBG );
}

//____________________________________________________________________________//

bool
wait_for_debugger()
{
    return s_const_arguments_store.get<bool>( WAIT_FOR_DEBUGGER );
}

//____________________________________________________________________________//

bool
use_alt_stack()
{
    return s_const_arguments_store.get<bool>( USE_ALT_STACK );
}

//____________________________________________________________________________//

bool
detect_fp_exceptions()
{
    return s_const_arguments_store.get<bool>( DETECT_FP_EXCEPT );
}

//____________________________________________________________________________//

unit_test::output_format
report_format()
{
    return s_arguments_store.get<unit_test::output_format>( REPORT_FORMAT );
}

//____________________________________________________________________________//

unit_test::output_format
log_format()
{
    return s_arguments_store.get<unit_test::output_format>( LOG_FORMAT );
}

//____________________________________________________________________________//

std::ostream*
report_sink()
{
    if( !s_arguments_store.has( REPORT_SINK ) )
        return &std::cerr;

    std::string const& sink_name = s_arguments_store.get<std::string>( REPORT_SINK );

    if( sink_name == "stderr" )
        return &std::cerr;

    if( sink_name == "stdout" )
        return &std::cout;

    static std::ofstream report_file( sink_name.c_str() );
    return &report_file;
}

//____________________________________________________________________________//

std::ostream*
log_sink()
{
    if( !s_arguments_store.has( LOG_SINK ) )
        return &std::cout;

    std::string const& sink_name = s_arguments_store.get<std::string>( LOG_SINK );

    if( sink_name == "stderr" )
        return &std::cerr;

    if( sink_name == "stdout" )
        return &std::cout;

    static std::ofstream log_file( sink_name.c_str() );
    return &log_file;
}

//____________________________________________________________________________//

// !!!!
#if 0
long
detect_memory_leaks()
{
    static long s_value = -1;

    if( s_value >= 0 )
        return s_value;

    std::string value = retrieve_argument( DETECT_MEM_LEAKS, s_empty );

    bool bool_val;
    if( rt::interpret_argument_value( value, bool_val ) )
        s_value = *bool_val ? 1L : 0L;
    else {
        BOOST_TEST_IMPL_TRY {
            // if representable as long - this is leak number
            s_value = boost::lexical_cast<long>( value );
        }
        BOOST_TEST_IMPL_CATCH0( boost::bad_lexical_cast ) {
            // value is leak report file and detection is enabled
            s_value = 1L;
        }
    }

    return s_value;
}

//____________________________________________________________________________//

const_string
memory_leaks_report_file()
{
    if( detect_memory_leaks() != 1 )
        return const_string();

    static std::string s_value;

    if( s_value.empty() ) {
        s_value = retrieve_argument<std::string>( DETECT_MEM_LEAKS );

        optional<bool> bool_val;
        if( rt::interpret_argument_value( s_value, bool_val ) )
            s_value.clear();
    }

    return s_value;
}

//____________________________________________________________________________//

#else

long
detect_memory_leaks()
{
    return 0;
}

const_string
memory_leaks_report_file()
{
    return const_string();
}

#endif

unsigned
random_seed()
{
    return s_const_arguments_store.get<unsigned>( RANDOM_SEED );
}

//____________________________________________________________________________//

} // namespace runtime_config
} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_UNIT_TEST_PARAMETERS_IPP_012205GER
