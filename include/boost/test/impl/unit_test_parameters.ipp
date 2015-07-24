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
#include <boost/test/utils/runtime/cla/parser.hpp>
#include <boost/test/utils/runtime/cla/parameter.hpp>
#include <boost/test/utils/runtime/interpret_argument_value.hpp>

namespace rt  = boost::runtime;
namespace cla = rt::cla;

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
#include <map>
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

// framework parameters and corresponding command-line arguments
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

static const_string
parameter_2_env_var( const_string param_name )
{
    typedef std::map<const_string,const_string> mtype;
    static mtype s_mapping;

    if( s_mapping.empty() ) {
        s_mapping[AUTO_START_DBG]       = "BOOST_TEST_AUTO_START_DBG";
        s_mapping[BREAK_EXEC_PATH]      = "BOOST_TEST_BREAK_EXEC_PATH";
        s_mapping[BUILD_INFO]           = "BOOST_TEST_BUILD_INFO";
        s_mapping[CATCH_SYS_ERRORS]     = "BOOST_TEST_CATCH_SYSTEM_ERRORS";
        s_mapping[COLOR_OUTPUT]         = "BOOST_TEST_COLOR_OUTPUT";
        s_mapping[DETECT_FP_EXCEPT]     = "BOOST_TEST_DETECT_FP_EXCEPTIONS";
        s_mapping[DETECT_MEM_LEAKS]     = "BOOST_TEST_DETECT_MEMORY_LEAK";
        s_mapping[LIST_CONTENT]         = "BOOST_TEST_LIST_CONTENT";
        s_mapping[LIST_CONTENT]         = "BOOST_TEST_LIST_LABELS";
        s_mapping[LOG_FORMAT]           = "BOOST_TEST_LOG_FORMAT";
        s_mapping[LOG_LEVEL]            = "BOOST_TEST_LOG_LEVEL";
        s_mapping[LOG_SINK]             = "BOOST_TEST_LOG_SINK";
        s_mapping[OUTPUT_FORMAT]        = "BOOST_TEST_OUTPUT_FORMAT";
        s_mapping[RANDOM_SEED]          = "BOOST_TEST_RANDOM";
        s_mapping[REPORT_FORMAT]        = "BOOST_TEST_REPORT_FORMAT";
        s_mapping[REPORT_LEVEL]         = "BOOST_TEST_REPORT_LEVEL";
        s_mapping[REPORT_SINK]          = "BOOST_TEST_REPORT_SINK";
        s_mapping[RESULT_CODE]          = "BOOST_TEST_RESULT_CODE";
        s_mapping[TESTS_TO_RUN]         = "BOOST_TESTS_TO_RUN";
        s_mapping[SAVE_TEST_PATTERN]    = "BOOST_TEST_SAVE_PATTERN";
        s_mapping[SHOW_PROGRESS]        = "BOOST_TEST_SHOW_PROGRESS";
        s_mapping[USE_ALT_STACK]        = "BOOST_TEST_USE_ALT_STACK";
        s_mapping[WAIT_FOR_DEBUGGER]    = "BOOST_TEST_WAIT_FOR_DEBUGGER";
    }

    mtype::const_iterator it = s_mapping.find( param_name );

    return it == s_mapping.end() ? const_string() : it->second;
}

 //____________________________________________________________________________//

cla::parser
make_cla_parser() 
{
// !!!!    cla::parser parser( cla::eop_marker="--" );
    cla::parser parser;

    cla::parameter<bool> auto_start_dbg( AUTO_START_DBG, 
        cla::description = "Automatically starts debugger if system level error (signal) occurs" );
    auto_start_dbg.add_id( "--", AUTO_START_DBG, "=" );
    auto_start_dbg.add_id( "-", "d", " " );
    parser.add( auto_start_dbg );

    cla::parameter<std::string> break_exec_path( BREAK_EXEC_PATH,
        cla::description = "For the exception safety testing allows to break at specific execution path" );
    break_exec_path.add_id( "--", BREAK_EXEC_PATH, "=" );    
    parser.add( break_exec_path );

    cla::parameter<bool> build_info( BUILD_INFO,
        cla::description = "Shows library build information" );
    build_info.add_id( "--", BUILD_INFO, "=" );
    build_info.add_id( "-", "i", " " );
    parser.add( build_info );

    cla::parameter<bool> catch_sys_errors( CATCH_SYS_ERRORS,
        cla::description = "Allows to switch between catching and ignoring system errors (signals)" );
    catch_sys_errors.add_id( "--", CATCH_SYS_ERRORS, "=" );
    catch_sys_errors.add_id( "-", "s", " " );
    parser.add( catch_sys_errors );

    cla::parameter<bool> color_output( COLOR_OUTPUT,
        cla::description = "Enables color output of the framework log and report messages" );
    color_output.add_id( "--", COLOR_OUTPUT, "=" );
    color_output.add_id( "-", "x", " " );
    parser.add( color_output );

    cla::parameter<bool> detect_fp_except( DETECT_FP_EXCEPT,
        cla::description = "Allows to switch between catching and ignoring floating point exceptions" );
    detect_fp_except.add_id( "--", DETECT_FP_EXCEPT, "=" );
    parser.add( detect_fp_except );

    cla::parameter<std::string> detect_mem_leaks( DETECT_MEM_LEAKS,
        cla::description = "Allows to switch between catching and ignoring memory leaks" );
    detect_mem_leaks.add_id( "--", DETECT_MEM_LEAKS, "=" );
    parser.add( detect_mem_leaks );

    cla::parameter<unit_test::output_format> list_content( LIST_CONTENT, (
        cla::optional_value,
        cla::description = "Lists the content of test tree - names of all test suites and test cases" ) );
    list_content.add_id( "--", LIST_CONTENT, "=" );    
    list_content.add_id( "-", "j", " " );
    parser.add( list_content );

    cla::parameter<bool> list_labels( LIST_LABELS,
        cla::description = "Lists all available labels" );
    list_labels.add_id( "--", LIST_LABELS, "=" );
    parser.add( list_labels );

    cla::parameter<unit_test::output_format> log_format( LOG_FORMAT,
        cla::description = "Specifies log format" );
    log_format.add_id( "--", LOG_FORMAT, "=" );
    log_format.add_id( "-", "f", " " );
    parser.add( log_format );

    cla::parameter<unit_test::log_level> log_level( LOG_LEVEL,
        cla::description = "Specifies log level" );
    log_level.add_id( "--", LOG_LEVEL, "=" );
    log_level.add_id( "-", "l", " " );
    parser.add( log_level );

    cla::parameter<std::string> log_sink( LOG_SINK,
        cla::description = "Specifies log sink: stdout(default),stderr or file name" );
    log_sink.add_id( "--", LOG_SINK, "=" );
    log_sink.add_id( "-", "k", " " );
    parser.add( log_sink );

    cla::parameter<unit_test::output_format> output_format( OUTPUT_FORMAT,
        cla::description = "Specifies output format (both log and report)" );
    output_format.add_id( "--", OUTPUT_FORMAT, "=" );
    output_format.add_id( "-", "o", " " );
    parser.add( output_format );

    cla::parameter<unsigned> random_seed( RANDOM_SEED, (
        cla::optional_value,
        cla::description = "Allows to switch between sequential and random order of test units execution.\n"
                           "Optionally allows to specify concrete seed for random number generator" ) );
    random_seed.add_id( "--", RANDOM_SEED, "=" );    
    random_seed.add_id( "-", "a", " " );
    parser.add( random_seed );

    cla::parameter<unit_test::output_format> report_format( REPORT_FORMAT,
        cla::description = "Specifies report format" );
    report_format.add_id( "--", REPORT_FORMAT, "=" );
    report_format.add_id( "-", "m", " " );
    parser.add( report_format );

    cla::parameter<unit_test::report_level> report_level( REPORT_LEVEL,
        cla::description = "Specifies report level" );
    report_level.add_id( "--", REPORT_LEVEL, "=" );
    report_level.add_id( "-", "r", " " );
    parser.add( report_level );

    cla::parameter<std::string> report_sink( REPORT_SINK,
        cla::description = "Specifies report sink: stderr(default),stdout or file name" );
    report_sink.add_id( "--", REPORT_SINK, "=" );
    report_sink.add_id( "-", "e", " " );
    parser.add( report_sink );

    cla::parameter<std::string> result_code( RESULT_CODE,
        cla::description = "Disables test modules's result code generation" );
    result_code.add_id( "--", RESULT_CODE, "=" );
    result_code.add_id( "-", "c", " " );
    parser.add( result_code );

    cla::parameter<std::string> tests_to_run( TESTS_TO_RUN, (
        cla::multiplicable,
    cla::description = "Filters, which test units to include or exclude from test module execution" ) );
    tests_to_run.add_id( "--", TESTS_TO_RUN, "=" );    
    tests_to_run.add_id( "-", "t", " " );
    parser.add( tests_to_run );

    cla::parameter<bool> save_test_pattern( SAVE_TEST_PATTERN, (
        cla::multiplicable,
        cla::description = "Allows to switch between saving and matching against test pattern file" ) );
    save_test_pattern.add_id( "--", SAVE_TEST_PATTERN, "=" );    
    parser.add( save_test_pattern );

    cla::parameter<bool> show_progress( SHOW_PROGRESS,
        cla::description = "Turns on progress display" );
    show_progress.add_id( "--", SHOW_PROGRESS, "=" );
    show_progress.add_id( "-", "p", " " );
    parser.add( show_progress );

    cla::parameter<bool> use_alt_stack( USE_ALT_STACK,
        cla::description = "Turns on/off usage of an alternative stack for signal handling" );
    use_alt_stack.add_id( "--", USE_ALT_STACK, "=" );
    parser.add( use_alt_stack );

    cla::parameter<bool> wait_for_debugger( WAIT_FOR_DEBUGGER,
        cla::description = "Forces test module to wait for button to be pressed before starting test run" );
    wait_for_debugger.add_id( "--", WAIT_FOR_DEBUGGER, "=" );
    wait_for_debugger.add_id( "-", "w", " " );
    parser.add( wait_for_debugger );

    cla::parameter<bool> help( "help",
        cla::description = "Help for framework parameters" );
    help.add_id( "--", "help", "=" );
    help.add_id( "-", "?", " " );
    parser.add( help );

    return parser;
}

static cla::parser&
cla_parser()
{
    static cla::parser s_instance = make_cla_parser();

    return s_instance;
}
static rt::argument_store s_argument_store;

std::string             s_empty;

output_format           s_report_format;
output_format           s_log_format;

std::list<std::string>  s_test_to_run;

//____________________________________________________________________________//

template<typename T>
T
retrieve_argument( std::string const& parameter_name, rt::argument_store const& store, T const& default_value = T(), T const& optional_value = T() )
{
    rt::const_argument_ptr arg = store[parameter_name];
    if( arg ) {
        if( rtti::type_id<T>() == rtti::type_id<bool>() ||
            !static_cast<cla::parameter const&>( arg->p_formal_parameter.get() ).p_optional_value )
            return parser.get<T>( parameter_name );

        optional<T> val = parser.get<optional<T> >( parameter_name );
        if( val )
            return *val;
        else
            return optional_value;
    }

    boost::optional<T> v;

#ifndef UNDER_CE
    env::get( parameter_2_env_var(parameter_name), v );
#endif

    if( v )
        return *v;
    else
        return default_value;
}

//____________________________________________________________________________//

// !!!!
#if 0
void
disable_use( cla::parameter const&, std::string const& )
{
    BOOST_TEST_SETUP_ASSERT( false, "parameter break_exec_path is disabled in this release" );
}
#endif

//____________________________________________________________________________//

} // local namespace

void
init( int& argc, char** argv )
{
    using namespace cla;

    BOOST_TEST_IMPL_TRY {
        cla_parser().parse( argc, argv, s_argument_store );

        if( s_argument_store["help"] ) {
            cla_parser().usage( std::cout );
            BOOST_TEST_IMPL_THROW( framework::nothing_to_test() );
        }

        s_report_format = retrieve_argument( REPORT_FORMAT, s_argument_store, unit_test::OF_CLF );
        s_log_format    = retrieve_argument( LOG_FORMAT, s_argument_store, unit_test::OF_CLF );
        output_format of = retrieve_argument( OUTPUT_FORMAT, s_argument_store, unit_test::OF_INVALID );

        if( of != unit_test::OF_INVALID )
            s_report_format = s_log_format = of;

        s_test_to_run = retrieve_argument<std::list<std::string> >( TESTS_TO_RUN, s_argument_store );
    }
    BOOST_TEST_IMPL_CATCH( rt::logic_error, ex ) {
        std::ostringstream err;

        err << "Fail to process runtime parameters: " << ex.msg() << std::endl;
        cla_parser().usage( err );

        BOOST_TEST_SETUP_ASSERT( false, err.str() );
    }
}

//____________________________________________________________________________//

unit_test::log_level
log_level()
{
    return retrieve_argument( LOG_LEVEL, s_argument_store, unit_test::log_all_errors );
}

//____________________________________________________________________________//

bool
no_result_code()
{
    return !retrieve_argument( RESULT_CODE, s_argument_store, true );
}

//____________________________________________________________________________//

unit_test::report_level
report_level()
{
    return retrieve_argument( REPORT_LEVEL, s_argument_store, unit_test::CONFIRMATION_REPORT );
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
    static std::string s_break_exec_path = retrieve_argument( BREAK_EXEC_PATH, s_argument_store, s_empty );

    return s_break_exec_path;
}

//____________________________________________________________________________//

bool
save_pattern()
{
    return retrieve_argument( SAVE_TEST_PATTERN, s_argument_store, false );
}

//____________________________________________________________________________//

bool
show_progress()
{
    return retrieve_argument( SHOW_PROGRESS, s_argument_store, false );
}

//____________________________________________________________________________//

bool
show_build_info()
{
    return retrieve_argument( BUILD_INFO, s_argument_store, false );
}

//____________________________________________________________________________//

output_format
list_content()
{
    return retrieve_argument( LIST_CONTENT, s_argument_store, unit_test::OF_INVALID, unit_test::OF_CLF );
}

//____________________________________________________________________________//

bool
list_labels()
{
    return retrieve_argument( LIST_LABELS, s_argument_store, false );
}

//____________________________________________________________________________//

bool
catch_sys_errors()
{
    return retrieve_argument( CATCH_SYS_ERRORS, s_argument_store,
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
    return retrieve_argument( COLOR_OUTPUT, s_argument_store, false );
}

//____________________________________________________________________________//

bool
auto_start_dbg()
{
    // !! ?? set debugger as an option
    return retrieve_argument( AUTO_START_DBG, s_argument_store, false );
;
}

//____________________________________________________________________________//

bool
wait_for_debugger()
{
    return retrieve_argument( WAIT_FOR_DEBUGGER, s_argument_store, false );
}

//____________________________________________________________________________//

bool
use_alt_stack()
{
    return retrieve_argument( USE_ALT_STACK, s_argument_store, true );
}

//____________________________________________________________________________//

bool
detect_fp_exceptions()
{
    return retrieve_argument( DETECT_FP_EXCEPT, s_argument_store, false );
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
    std::string sink_name = retrieve_argument( REPORT_SINK, s_argument_store, s_empty );

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
    std::string sink_name = retrieve_argument( LOG_SINK, s_argument_store, s_empty );

    if( sink_name.empty() || sink_name == "stdout" )
        return &std::cout;

    if( sink_name == "stderr" )
        return &std::cerr;

    static std::ofstream log_file( sink_name.c_str() );
    return &log_file;
}

//____________________________________________________________________________//

long
detect_memory_leaks()
{
    static long s_value = -1;

    if( s_value >= 0 )
        return s_value;

    std::string value = retrieve_argument( DETECT_MEM_LEAKS, s_argument_store, s_empty );

    optional<bool> bool_val;
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
        s_value = retrieve_argument<std::string>( DETECT_MEM_LEAKS, s_argument_store );

        optional<bool> bool_val;
        if( rt::interpret_argument_value( s_value, bool_val ) )
            s_value.clear();
    }

    return s_value;
}

//____________________________________________________________________________//

unsigned
random_seed()
{
    return retrieve_argument( RANDOM_SEED, s_argument_store, 0U, 1U );
}

//____________________________________________________________________________//

} // namespace runtime_config
} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_UNIT_TEST_PARAMETERS_IPP_012205GER
