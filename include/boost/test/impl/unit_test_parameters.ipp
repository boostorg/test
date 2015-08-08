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
#include <boost/test/utils/runtime/interpret_argument_value.hpp>

#include <boost/test/utils/runtime/cla/parser.hpp>

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
    static fixed_mapping<const_string,unit_test::log_level,case_ins_less<char const> > log_level_name(
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
    BOOST_TEST_SETUP_ASSERT( ll != unit_test::invalid_log_level, "invalid log level " + val );

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
        "HRF", unit_test::OF_CLF,
        "CLF", unit_test::OF_CLF,
        "XML", unit_test::OF_XML,
        "DOT", unit_test::OF_DOT,

        unit_test::OF_INVALID
        );

    std::string val;
    in >> val;

    of = output_format_name[val];
    BOOST_TEST_SETUP_ASSERT( of != unit_test::OF_INVALID, "invalid output format " + val );

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
std::string TESTS_TO_RUN      = "run_test";
std::string SAVE_TEST_PATTERN = "save_pattern";
std::string SHOW_PROGRESS     = "show_progress";
std::string USE_ALT_STACK     = "use_alt_stack";
std::string WAIT_FOR_DEBUGGER = "wait_for_debugger";

//____________________________________________________________________________//

void
register_parameters( rt::parameters_store& store )
{
    rt::parameter<bool> auto_start_dbg( AUTO_START_DBG, (
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

    rt::parameter<bool> build_info( BUILD_INFO, (
        rt::description = "Shows library build information",
        rt::env_var = "BOOST_TEST_BUILD_INFO"
    ));

    build_info.add_cla_id( "--", BUILD_INFO, "=" );
    build_info.add_cla_id( "-", "i", " " );
    store.add( build_info );

    rt::parameter<bool> catch_sys_errors( CATCH_SYS_ERRORS, (
        rt::description = "Allows to switch between catching and ignoring system errors (signals)",
        rt::env_var = "BOOST_TEST_CATCH_SYSTEM_ERRORS"
    ));

    catch_sys_errors.add_cla_id( "--", CATCH_SYS_ERRORS, "=" );
    catch_sys_errors.add_cla_id( "-", "s", " " );
    store.add( catch_sys_errors );

    rt::parameter<bool> color_output( COLOR_OUTPUT, (
        rt::description = "Enables color output of the framework log and report messages",
        rt::env_var = "BOOST_TEST_COLOR_OUTPUT"
    ));

    color_output.add_cla_id( "--", COLOR_OUTPUT, "=" );
    color_output.add_cla_id( "-", "x", " " );
    store.add( color_output );

    rt::parameter<bool> detect_fp_except( DETECT_FP_EXCEPT, (
        rt::description = "Allows to switch between catching and ignoring floating point exceptions",
        rt::env_var = "BOOST_TEST_DETECT_FP_EXCEPTIONS"
    ));

    detect_fp_except.add_cla_id( "--", DETECT_FP_EXCEPT, "=" );
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
        rt::env_var = "BOOST_TEST_LIST_CONTENT"
    ));
    list_content.add_cla_id( "--", LIST_CONTENT, "=" );    
    store.add( list_content );

    rt::parameter<bool> list_labels( LIST_LABELS, (
        rt::description = "Lists all available labels",
        rt::env_var = "BOOST_TEST_LIST_LABELS"
    ));

    list_labels.add_cla_id( "--", LIST_LABELS, "=" );
    store.add( list_labels );

    rt::parameter<unit_test::output_format> log_format( LOG_FORMAT, (
        rt::description = "Specifies log format",
        rt::env_var = "BOOST_TEST_LOG_FORMAT"
    ));

    log_format.add_cla_id( "--", LOG_FORMAT, "=" );
    log_format.add_cla_id( "-", "f", " " );
    store.add( log_format );

    rt::parameter<unit_test::log_level> log_level( LOG_LEVEL, (
        rt::description = "Specifies log level",
        rt::env_var = "BOOST_TEST_LOG_LEVEL"
    ));

    log_level.add_cla_id( "--", LOG_LEVEL, "=" );
    log_level.add_cla_id( "-", "l", " " );
    store.add( log_level );

    rt::parameter<std::string> log_sink( LOG_SINK, (
        rt::description = "Specifies log sink: stdout(default),stderr or file name",
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
        rt::env_var = "BOOST_TEST_RANDOM"
    ));

    random_seed.add_cla_id( "--", RANDOM_SEED, "=" );    
    store.add( random_seed );

    rt::parameter<unit_test::output_format> report_format( REPORT_FORMAT, (
        rt::description = "Specifies report format",
        rt::env_var = "BOOST_TEST_REPORT_FORMAT"
    ));

    report_format.add_cla_id( "--", REPORT_FORMAT, "=" );
    report_format.add_cla_id( "-", "m", " " );
    store.add( report_format );

    rt::parameter<unit_test::report_level> report_level( REPORT_LEVEL, (
        rt::description = "Specifies report level",
        rt::env_var = "BOOST_TEST_REPORT_LEVEL"
    ));

    report_level.add_cla_id( "--", REPORT_LEVEL, "=" );
    report_level.add_cla_id( "-", "r", " " );
    store.add( report_level );

    rt::parameter<std::string> report_sink( REPORT_SINK, (
        rt::description = "Specifies report sink: stderr(default),stdout or file name",
        rt::env_var = "BOOST_TEST_REPORT_SINK"
    ));

    report_sink.add_cla_id( "--", REPORT_SINK, "=" );
    report_sink.add_cla_id( "-", "e", " " );
    store.add( report_sink );

    rt::parameter<std::string> result_code( RESULT_CODE, (
        rt::description = "Disables test modules's result code generation",
        rt::env_var = "BOOST_TEST_RESULT_CODE"
    ));

    result_code.add_cla_id( "--", RESULT_CODE, "=" );
    result_code.add_cla_id( "-", "c", " " );
    store.add( result_code );

    rt::parameter<std::string,rt::REPEATABLE> tests_to_run( TESTS_TO_RUN, (
        rt::description = "Filters, which test units to include or exclude from test module execution",
        rt::env_var = "BOOST_TESTS_TO_RUN"
    ));

    tests_to_run.add_cla_id( "--", TESTS_TO_RUN, "=" );    
    tests_to_run.add_cla_id( "-", "t", " " );
    store.add( tests_to_run );

    rt::parameter<bool> save_test_pattern( SAVE_TEST_PATTERN, (
        rt::description = "Allows to switch between saving and matching against test pattern file",
        rt::env_var = "BOOST_TEST_SAVE_PATTERN"
    ));

    save_test_pattern.add_cla_id( "--", SAVE_TEST_PATTERN, "=" );    
    store.add( save_test_pattern );

    rt::parameter<bool> show_progress( SHOW_PROGRESS, (
        rt::description = "Turns on progress display",
        rt::env_var = "BOOST_TEST_SHOW_PROGRESS"
    ));

    show_progress.add_cla_id( "--", SHOW_PROGRESS, "=" );
    show_progress.add_cla_id( "-", "p", " " );
    store.add( show_progress );

    rt::parameter<bool> use_alt_stack( USE_ALT_STACK, (
        rt::description = "Turns on/off usage of an alternative stack for signal handling",
        rt::env_var = "BOOST_TEST_USE_ALT_STACK"
    ));

    use_alt_stack.add_cla_id( "--", USE_ALT_STACK, "=" );
    store.add( use_alt_stack );

    rt::parameter<bool> wait_for_debugger( WAIT_FOR_DEBUGGER, (
        rt::description = "Forces test module to wait for button to be pressed before starting test run",
        rt::env_var = "BOOST_TEST_WAIT_FOR_DEBUGGER"
    ));

    wait_for_debugger.add_cla_id( "--", WAIT_FOR_DEBUGGER, "=" );
    wait_for_debugger.add_cla_id( "-", "w", " " );
    store.add( wait_for_debugger );

    rt::parameter<bool> help( "help",
        rt::description = "Help for framework parameters" );
    help.add_cla_id( "--", "help", "=" );
    help.add_cla_id( "-", "?", " " );
    store.add( help );
}

static rt::arguments_store  s_arguments_store;
static rt::parameters_store s_parameters_store;

std::string             s_empty;

output_format           s_report_format;
output_format           s_log_format;

std::list<std::string>  s_test_to_run;

//____________________________________________________________________________//

template<typename T>
T
retrieve_argument( std::string const& parameter_name, T const& default_value = T(), T const& optional_value = T() )
{
// !!!!
#if 0 
    rt::arguments_store::const_iterator found = s_arguments_store.find( parameter_name );
    if( found != s_arguments_store.end() ) {
        rt::const_argument_ptr arg = found->second;

        if( rtti::type_id<T>() == rtti::type_id<bool>() ||
            !static_cast<rt::parameter const&>( arg->p_formal_parameter.get() ).p_optional_value )
            return store.get<T>( parameter_name );

        optional<T> val = store.get<optional<T> >( parameter_name );
        if( val )
            return *val;
        return optional_value;
    }

    boost::optional<T> v;

#ifndef UNDER_CE
    env::get( parameter_2_env_var(parameter_name), v );
#endif

    if( v )
        return *v;
#endif

    return default_value;
}

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
        if( s_parameters_store.is_empty() )
            register_parameters( s_parameters_store );

        s_arguments_store.clear();

        // !!!!    cla::parser parser( cla::eop_marker="--" );

        rt::cla::parser parser( s_parameters_store );

        parser.parse( argc, argv, s_arguments_store );

        if( s_arguments_store.has("help") ) {
            // !!!! parser.usage( s_parameters_store, std::cout );
            BOOST_TEST_IMPL_THROW( framework::nothing_to_test() );
        }

        s_report_format = retrieve_argument( REPORT_FORMAT, unit_test::OF_CLF );
        s_log_format    = retrieve_argument( LOG_FORMAT, unit_test::OF_CLF );
        output_format of = retrieve_argument( OUTPUT_FORMAT, unit_test::OF_INVALID );

        if( of != unit_test::OF_INVALID )
            s_report_format = s_log_format = of;

        s_test_to_run = retrieve_argument<std::list<std::string> >( TESTS_TO_RUN );
    }
    BOOST_TEST_IMPL_CATCH( rt::init_error, ex ) {
        BOOST_TEST_SETUP_ASSERT( false, "Internal Boost.Test initialization error: " + ex.msg );
    }
    BOOST_TEST_IMPL_CATCH( rt::parse_error, ex ) {
        // !!!! help/usage?
        BOOST_TEST_SETUP_ASSERT( false, ex.msg );
    }
}

//____________________________________________________________________________//

unit_test::log_level
log_level()
{
    return retrieve_argument( LOG_LEVEL, unit_test::log_all_errors );
}

//____________________________________________________________________________//

bool
no_result_code()
{
    return !retrieve_argument( RESULT_CODE, true );
}

//____________________________________________________________________________//

unit_test::report_level
report_level()
{
    return retrieve_argument( REPORT_LEVEL, unit_test::CONFIRMATION_REPORT );
}

//____________________________________________________________________________//

std::list<std::string> const&
test_to_run()
{
    return s_test_to_run;
}

//____________________________________________________________________________//

const_string
break_exec_path()
{
    static std::string s_break_exec_path = retrieve_argument( BREAK_EXEC_PATH, s_empty );

    return s_break_exec_path;
}

//____________________________________________________________________________//

bool
save_pattern()
{
    return retrieve_argument( SAVE_TEST_PATTERN, false );
}

//____________________________________________________________________________//

bool
show_progress()
{
    return retrieve_argument( SHOW_PROGRESS, false );
}

//____________________________________________________________________________//

bool
show_build_info()
{
    return retrieve_argument( BUILD_INFO, false );
}

//____________________________________________________________________________//

output_format
list_content()
{
    return retrieve_argument( LIST_CONTENT, unit_test::OF_INVALID, unit_test::OF_CLF );
}

//____________________________________________________________________________//

bool
list_labels()
{
    return retrieve_argument( LIST_LABELS, false );
}

//____________________________________________________________________________//

bool
catch_sys_errors()
{
    return retrieve_argument( CATCH_SYS_ERRORS,
#ifdef BOOST_TEST_DEFAULTS_TO_CORE_DUMP
        false
#else
        true
#endif
        );
}

//____________________________________________________________________________//

bool
color_output()
{
    return retrieve_argument( COLOR_OUTPUT, false );
}

//____________________________________________________________________________//

bool
auto_start_dbg()
{
    // !! ?? set debugger as an option
    return retrieve_argument( AUTO_START_DBG, false );
;
}

//____________________________________________________________________________//

bool
wait_for_debugger()
{
    return retrieve_argument( WAIT_FOR_DEBUGGER, false );
}

//____________________________________________________________________________//

bool
use_alt_stack()
{
    return retrieve_argument( USE_ALT_STACK, true );
}

//____________________________________________________________________________//

bool
detect_fp_exceptions()
{
    return retrieve_argument( DETECT_FP_EXCEPT, false );
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

std::ostream*
report_sink()
{
    std::string sink_name = retrieve_argument( REPORT_SINK, s_empty );

    if( sink_name.empty() || sink_name == "stderr" )
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
    std::string sink_name = retrieve_argument( LOG_SINK, s_empty );

    if( sink_name.empty() || sink_name == "stdout" )
        return &std::cout;

    if( sink_name == "stderr" )
        return &std::cerr;

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
    return retrieve_argument( RANDOM_SEED, 0U, 1U );
}

//____________________________________________________________________________//

} // namespace runtime_config
} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_UNIT_TEST_PARAMETERS_IPP_012205GER
