//  (C) Copyright Gennadiy Rozental 2001.
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
#include <boost/test/debug.hpp>
#include <boost/test/framework.hpp>

#include <boost/test/detail/throw_exception.hpp>

// Boost.Runtime.Param
#include <boost/test/utils/runtime/parameter.hpp>
#include <boost/test/utils/runtime/argument.hpp>
#include <boost/test/utils/runtime/finalize.hpp>
#include <boost/test/utils/runtime/cla/parser.hpp>
#include <boost/test/utils/runtime/env/fetch.hpp>

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

namespace rt = boost::runtime;

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
std::string REPORT_MEM_LEAKS  = "report_memory_leaks_to";
std::string REPORT_SINK       = "report_sink";
std::string RESULT_CODE       = "result_code";
std::string RUN_FILTERS       = "run_test";
std::string SAVE_TEST_PATTERN = "save_pattern";
std::string SHOW_PROGRESS     = "show_progress";
std::string USE_ALT_STACK     = "use_alt_stack";
std::string WAIT_FOR_DEBUGGER = "wait_for_debugger";

std::string HELP              = "help";
std::string USAGE             = "usage";

//____________________________________________________________________________//

void
register_parameters( rt::parameters_store& store )
{
    rt::option auto_start_dbg( AUTO_START_DBG, (
        rt::description = "Automatically starts debugger if system level error (signal) occurs.",
        rt::env_var = "BOOST_TEST_AUTO_START_DBG"
    ));

    auto_start_dbg.add_cla_id( "--", AUTO_START_DBG, "=" );
    auto_start_dbg.add_cla_id( "-", "d", " " );
    store.add( auto_start_dbg );

    rt::parameter<std::string> break_exec_path( BREAK_EXEC_PATH, (
        rt::description = "For the exception safety testing allows to break at specific execution path.",
        rt::env_var = "BOOST_TEST_BREAK_EXEC_PATH"
    ));

    break_exec_path.add_cla_id( "--", BREAK_EXEC_PATH, "=" );    
    store.add( break_exec_path );

    rt::option build_info( BUILD_INFO, (
        rt::description = "Shows library build information.",
        rt::env_var = "BOOST_TEST_BUILD_INFO"
    ));

    build_info.add_cla_id( "--", BUILD_INFO, "=" );
    build_info.add_cla_id( "-", "i", " " );
    store.add( build_info );


    rt::option catch_sys_errors( CATCH_SYS_ERRORS, (
        rt::description = "Allows to switch between catching and ignoring system errors (signals).",
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
        rt::description = "Enables color output of the framework log and report messages.",
        rt::env_var = "BOOST_TEST_COLOR_OUTPUT"
    ));

    color_output.add_cla_id( "--", COLOR_OUTPUT, "=", true );
    color_output.add_cla_id( "-", "x", " " );
    store.add( color_output );

    rt::option detect_fp_except( DETECT_FP_EXCEPT, (
        rt::description = "Allows to switch between catching and ignoring floating point exceptions.",
        rt::env_var = "BOOST_TEST_DETECT_FP_EXCEPTIONS"
    ));

    detect_fp_except.add_cla_id( "--", DETECT_FP_EXCEPT, "=", true );
    store.add( detect_fp_except );

    rt::parameter<long> detect_mem_leaks( DETECT_MEM_LEAKS, (
        rt::description = "Turns on/off memory leaks detection (optionally breaking on specified alloc order number).",
        rt::env_var = "BOOST_TEST_DETECT_MEMORY_LEAK",
        rt::default_value = 0L,
        rt::optional_value = 1L,
        rt::value_hint = "<alloc order number>"
    ));
    detect_mem_leaks.add_cla_id( "--", DETECT_MEM_LEAKS, "=" );
    store.add( detect_mem_leaks );

    rt::enum_parameter<unit_test::output_format> list_content( LIST_CONTENT, (
        rt::description = "Lists the content of test tree - names of all test suites and test cases.",
        rt::env_var = "BOOST_TEST_LIST_CONTENT",
        rt::default_value = OF_INVALID,
        rt::optional_value = OF_CLF,
        rt::enum_values<unit_test::output_format>::value = {
            { "HRF", OF_CLF },
            { "DOT", OF_DOT }
        }
    ));
    list_content.add_cla_id( "--", LIST_CONTENT, "=" );
    store.add( list_content );

    rt::option list_labels( LIST_LABELS, (
        rt::description = "Lists all available labels.",
        rt::env_var = "BOOST_TEST_LIST_LABELS"
    ));

    list_labels.add_cla_id( "--", LIST_LABELS, "=" );
    store.add( list_labels );

    rt::enum_parameter<unit_test::output_format> log_format( LOG_FORMAT, (
        rt::description = "Specifies log format.",
        rt::env_var = "BOOST_TEST_LOG_FORMAT",
        rt::default_value = OF_CLF,
        rt::enum_values<unit_test::output_format>::value = {
            { "HRF", OF_CLF },
            { "CLF", OF_CLF },
            { "XML", OF_XML }
        }
    ));

    log_format.add_cla_id( "--", LOG_FORMAT, "=" );
    log_format.add_cla_id( "-", "f", " " );
    store.add( log_format );

    rt::enum_parameter<unit_test::log_level> log_level( LOG_LEVEL, (
        rt::description = "Specifies log level.",
        rt::env_var = "BOOST_TEST_LOG_LEVEL",
        rt::default_value = log_all_errors,
        rt::enum_values<unit_test::log_level>::value = {
            { "all"           , log_successful_tests },
            { "success"       , log_successful_tests },
            { "test_suite"    , log_test_units },
            { "unit_scope"    , log_test_units },
            { "message"       , log_messages },
            { "warning"       , log_warnings },
            { "error"         , log_all_errors },
            { "cpp_exception" , log_cpp_exception_errors },
            { "system_error"  , log_system_errors },
            { "fatal_error"   , log_fatal_errors },
            { "nothing"       , log_nothing }
        }
    ));

    log_level.add_cla_id( "--", LOG_LEVEL, "=" );
    log_level.add_cla_id( "-", "l", " " );
    store.add( log_level );

    rt::parameter<std::string> log_sink( LOG_SINK, (
        rt::description = "Specifies log sink: stdout(default), stderr or file name.",
        rt::env_var = "BOOST_TEST_LOG_SINK",
        rt::value_hint = "<stderr|stdout|file name>"
    ));

    log_sink.add_cla_id( "--", LOG_SINK, "=" );
    log_sink.add_cla_id( "-", "k", " " );
    store.add( log_sink );

    rt::enum_parameter<unit_test::output_format> output_format( OUTPUT_FORMAT, (
        rt::description = "Specifies output format (both log and report).",
        rt::env_var = "BOOST_TEST_OUTPUT_FORMAT",
        rt::enum_values<unit_test::output_format>::value = {
            { "HRF", OF_CLF },
            { "CLF", OF_CLF },
            { "XML", OF_XML }
        }
    ));

    output_format.add_cla_id( "--", OUTPUT_FORMAT, "=" );
    output_format.add_cla_id( "-", "o", " " );
    store.add( output_format );

    rt::parameter<unsigned> random_seed( RANDOM_SEED, (
        rt::description = "Allows to switch between sequential and random order of test units execution."
                          " Optionally allows to specify concrete seed for random number generator.",
        rt::env_var = "BOOST_TEST_RANDOM",
        rt::default_value = 0U,
        rt::optional_value = 1U,
        rt::value_hint = "<seed>"
    ));

    random_seed.add_cla_id( "--", RANDOM_SEED, "=" );
    store.add( random_seed );

    rt::enum_parameter<unit_test::output_format> report_format( REPORT_FORMAT, (
        rt::description = "Specifies report format.",
        rt::env_var = "BOOST_TEST_REPORT_FORMAT",
        rt::default_value = OF_CLF,
        rt::enum_values<unit_test::output_format>::value = {
            { "HRF", OF_CLF },
            { "CLF", OF_CLF },
            { "XML", OF_XML }
        }
    ));

    report_format.add_cla_id( "--", REPORT_FORMAT, "=" );
    report_format.add_cla_id( "-", "m", " " );
    store.add( report_format );

    rt::enum_parameter<unit_test::report_level> report_level( REPORT_LEVEL, (
        rt::description = "Specifies report level.",
        rt::env_var = "BOOST_TEST_REPORT_LEVEL",
        rt::default_value = CONFIRMATION_REPORT,
        rt::enum_values<unit_test::report_level>::value = {
            { "confirm",  CONFIRMATION_REPORT },
            { "short",    SHORT_REPORT },
            { "detailed", DETAILED_REPORT },
            { "no",       NO_REPORT }
        }
    ));

    report_level.add_cla_id( "--", REPORT_LEVEL, "=" );
    report_level.add_cla_id( "-", "r", " " );
    store.add( report_level );

    rt::parameter<std::string> report_sink( REPORT_SINK, (
        rt::description = "Specifies report sink: stderr(default), stdout or file name.",
        rt::env_var = "BOOST_TEST_REPORT_SINK",
        rt::value_hint = "<stderr|stdout|file name>"
    ));

    rt::parameter<std::string> report_mem_leaks( REPORT_MEM_LEAKS, (
        rt::description = "File where to repport memory leaks to.",
        rt::env_var = "BOOST_TEST_REPORT_MEMORY_LEAK",
        rt::default_value = std::string(),
        rt::value_hint = "<file name>"
    ));
    report_mem_leaks.add_cla_id( "--", REPORT_MEM_LEAKS, "=" );
    store.add( report_mem_leaks );

    report_sink.add_cla_id( "--", REPORT_SINK, "=" );
    report_sink.add_cla_id( "-", "e", " " );
    store.add( report_sink );

    rt::option result_code( RESULT_CODE, (
        rt::description = "Disables test modules's result code generation.",
        rt::env_var = "BOOST_TEST_RESULT_CODE",
        rt::default_value = true
    ));

    result_code.add_cla_id( "--", RESULT_CODE, "=", true );
    result_code.add_cla_id( "-", "c", " " );
    store.add( result_code );

    rt::parameter<std::string,rt::REPEATABLE_PARAM> tests_to_run( RUN_FILTERS, (
        rt::description = "Filters, which test units to include or exclude from test module execution.",
        rt::env_var = "BOOST_TEST_RUN_FILTERS",
        rt::value_hint = "<test unit filter>"
    ));

    tests_to_run.add_cla_id( "--", RUN_FILTERS, "=" );
    tests_to_run.add_cla_id( "-", "t", " " );
    store.add( tests_to_run );

    rt::option save_test_pattern( SAVE_TEST_PATTERN, (
        rt::description = "Allows to switch between saving and matching against test pattern file.",
        rt::env_var = "BOOST_TEST_SAVE_PATTERN"
    ));

    save_test_pattern.add_cla_id( "--", SAVE_TEST_PATTERN, "=" );
    store.add( save_test_pattern );

    rt::option show_progress( SHOW_PROGRESS, (
        rt::description = "Turns on progress display.",
        rt::env_var = "BOOST_TEST_SHOW_PROGRESS"
    ));

    show_progress.add_cla_id( "--", SHOW_PROGRESS, "=" );
    show_progress.add_cla_id( "-", "p", " " );
    store.add( show_progress );

    rt::option use_alt_stack( USE_ALT_STACK, (
        rt::description = "Turns on/off usage of an alternative stack for signal handling.",
        rt::env_var = "BOOST_TEST_USE_ALT_STACK",
        rt::default_value = true
    ));

    use_alt_stack.add_cla_id( "--", USE_ALT_STACK, "=", true );
    store.add( use_alt_stack );

    rt::option wait_for_debugger( WAIT_FOR_DEBUGGER, (
        rt::description = "Forces test module to wait for button to be pressed before starting test run.",
        rt::env_var = "BOOST_TEST_WAIT_FOR_DEBUGGER"
    ));

    wait_for_debugger.add_cla_id( "--", WAIT_FOR_DEBUGGER, "=" );
    wait_for_debugger.add_cla_id( "-", "w", " " );
    store.add( wait_for_debugger );

    rt::parameter<std::string> help( HELP, (
        rt::description = "Help for framework parameters.",
        rt::optional_value = std::string(),
        rt::value_hint = "<parameter name>"
    ));
    help.add_cla_id( "--", HELP, "=" );
    store.add( help );

    rt::option usage( USAGE, (
        rt::description = "Short message explaining usage of Boost.Test parameters."
    ));
    usage.add_cla_id( "-", "?", " " );
    store.add( usage );
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
    shared_ptr<rt::cla::parser> parser;

    BOOST_TEST_I_TRY {
        // Initialize parameters list
        if( s_parameters_store.is_empty() )
            register_parameters( s_parameters_store );

        // Clear up arguments store just in case (of multiple init invocations)
        s_arguments_store.clear();

        // Parse CLA they take precedence over  environment
        parser.reset( new rt::cla::parser( s_parameters_store, (rt::end_of_params = "--", rt::negation_prefix = "no_") ) );
        parser->parse( argc, argv, s_arguments_store );

        // Try to fetch missing arguments from environment
        rt::env::fetch_absent( s_parameters_store, s_arguments_store );

        // Set arguments to default values if defined and perform all the validations
        rt::finalize_arguments( s_parameters_store, s_arguments_store );

        // Report help if requested
        if( s_const_arguments_store.get<bool>( USAGE ) ) {
            parser->usage( std::cerr );
            BOOST_TEST_I_THROW( framework::nothing_to_test() );
        }
        else if( s_const_arguments_store.has( HELP ) ) {
            parser->help( std::cerr, s_parameters_store, s_const_arguments_store.get<std::string>( HELP ) );
            BOOST_TEST_I_THROW( framework::nothing_to_test() );
        }

        // A bit of business logic: output_format takes precedence over log/report formats
        if( s_arguments_store.has( OUTPUT_FORMAT ) ) {
            unit_test::output_format of = s_arguments_store.get<unit_test::output_format>( OUTPUT_FORMAT );
            s_arguments_store.set( REPORT_FORMAT, of );
            s_arguments_store.set( LOG_FORMAT, of );
        }
    }
    BOOST_TEST_I_CATCH( rt::init_error, ex ) {
        BOOST_TEST_SETUP_ASSERT( false, ex.msg );
    }
    BOOST_TEST_I_CATCH( rt::ambiguous_param, ex ) {
        std::cerr << ex.msg << "\n Did you mean one of these?\n";

        BOOST_TEST_FOREACH( rt::cstring, name, ex.m_amb_candidates )
            std::cerr << "   " << name << "\n";

        std::cerr << "\n";

        if( parser )
            parser->usage( std::cerr );
        
        BOOST_TEST_I_THROW( framework::nothing_to_test() );
    }
    BOOST_TEST_I_CATCH( rt::unrecognized_param, ex ) {
        std::cerr << ex.msg << "\n";

        if( !ex.m_typo_candidates.empty() ) {
            std::cerr << " Did you mean one of these?\n";

            BOOST_TEST_FOREACH( rt::cstring, name, ex.m_typo_candidates )
                std::cerr << "   " << name << "\n";
        }

        std::cerr << "\n";

        if( parser )
            parser->usage( std::cerr );
        
        BOOST_TEST_I_THROW( framework::nothing_to_test() );
    }
    BOOST_TEST_I_CATCH( rt::input_error, ex ) {
        std::cerr << ex.msg << "\n\n";

        if( parser )
            parser->usage( std::cerr );
        
        BOOST_TEST_I_THROW( framework::nothing_to_test() );
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

long
detect_memory_leaks()
{
    return s_const_arguments_store.get<long>( DETECT_MEM_LEAKS );
}

//____________________________________________________________________________//

const_string
memory_leaks_report_file()
{
    return s_const_arguments_store.get<std::string>( REPORT_MEM_LEAKS );
}

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
