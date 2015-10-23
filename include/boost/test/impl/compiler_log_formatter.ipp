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
//  Description : implements compiler like Log formatter
// ***************************************************************************

#ifndef BOOST_TEST_COMPILER_LOG_FORMATTER_IPP_020105GER
#define BOOST_TEST_COMPILER_LOG_FORMATTER_IPP_020105GER

// Boost.Test
#include <boost/test/framework.hpp>
#include <boost/test/execution_monitor.hpp>
#include <boost/test/tree/test_unit.hpp>
#include <boost/test/results_collector.hpp>
#include <boost/test/utils/basic_cstring/io.hpp>
#include <boost/test/utils/lazy_ostream.hpp>
#include <boost/test/utils/setcolor.hpp>
#include <boost/test/output/compiler_log_formatter.hpp>
#include <boost/test/unit_test_parameters.hpp>

// Boost
#include <boost/version.hpp>

// STL
#include <iostream>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace unit_test {
namespace output {

// ************************************************************************** //
// **************            compiler_log_formatter            ************** //
// ************************************************************************** //

namespace {

std::string
test_phase_identifier()
{
    return framework::test_in_progress() ? framework::current_test_case().full_name() : std::string( "Test setup" );
}

//____________________________________________________________________________//

void
print_result( std::ostream& ostr, counter_t v, counter_t total,
              const_string name, const_string res )
{
    if( v > 0 ) {
        ostr << v << ' ' << name << ( v != 1 ? "s" : "" );
        if( total > 0 )
            ostr << " out of " << total;

        ostr << ' ' << res << ". ";
    }
}


} // local namespace

//____________________________________________________________________________//

void
compiler_log_formatter::log_start( std::ostream& output, counter_t test_cases_amount )
{
    if( test_cases_amount > 0 )
        output  << "Running " << test_cases_amount << " test "
                << (test_cases_amount > 1 ? "cases" : "case") << "...\n";
}

//____________________________________________________________________________//

void
compiler_log_formatter::log_finish( std::ostream& ostr )
{
    ostr.flush();
}

//____________________________________________________________________________//

void
compiler_log_formatter::log_build_info( std::ostream& output )
{
    output  << "Platform: " << BOOST_PLATFORM            << '\n'
            << "Compiler: " << BOOST_COMPILER            << '\n'
            << "STL     : " << BOOST_STDLIB              << '\n'
            << "Boost   : " << BOOST_VERSION/100000      << "."
                            << BOOST_VERSION/100 % 1000  << "."
                            << BOOST_VERSION % 100       << std::endl;
}

//____________________________________________________________________________//

void
compiler_log_formatter::test_unit_start( std::ostream& output, test_unit const& tu )
{
    BOOST_TEST_SCOPE_SETCOLOR( output, term_attr::BRIGHT, term_color::BLUE );

    print_prefix( output, tu.p_file_name, tu.p_line_num );

    output << "Entering test " << tu.p_type_name << " \"" << tu.p_name << "\"" << std::endl;
}

//____________________________________________________________________________//

void
compiler_log_formatter::test_unit_finish( std::ostream& output, test_unit const& tu, unsigned long elapsed )
{
    BOOST_TEST_SCOPE_SETCOLOR( output, term_attr::BRIGHT, term_color::BLUE );

    print_prefix( output, tu.p_file_name, tu.p_line_num );

    output << "Leaving test " << tu.p_type_name << " \"" << tu.p_name << "\"";

    if( elapsed > 0 ) {
        output << "; testing time: ";
        if( elapsed % 1000 == 0 )
            output << elapsed/1000 << "ms";
        else
            output << elapsed << "us";
    }

    output << ". ";

    test_results const& tr = results_collector.results( tu.p_id );

    const_string status;

    if( tr.passed() )
        status = "passed";
    else if( tr.p_skipped )
        status = "skipped";
    else if( tr.p_aborted )
        status = "aborted";
    else
        status = "failed";

    output << "Test " << ( tu.p_type == TUT_CASE ? "case" : "suite" ) << ' ' << status << ". ";

    if( tr.p_skipped ) {
        output << "due to " << ( tu.check_dependencies() ? "test aborting\n" : "failed dependency\n" );
        return;
    }

    counter_t total_assertions  = tr.p_assertions_passed + tr.p_assertions_failed;
    counter_t total_tc          = tr.p_test_cases_passed + tr.p_test_cases_warned + tr.p_test_cases_failed + tr.p_test_cases_skipped;

    if( total_assertions > 0 || total_tc > 0 ) {
        print_result( output, tr.p_assertions_passed,  total_assertions, "assertion", "passed" );
        print_result( output, tr.p_assertions_failed,  total_assertions, "assertion", "failed" );
        print_result( output, tr.p_warnings_failed,    0               , "failure"  , "warning" );
        print_result( output, tr.p_expected_failures,  0               , "failure"  , "expected" );
        print_result( output, tr.p_test_cases_passed,  total_tc        , "test case", "passed" );
        print_result( output, tr.p_test_cases_warned,  total_tc        , "test case", "warned" );
        print_result( output, tr.p_test_cases_failed,  total_tc        , "test case", "failed" );
        print_result( output, tr.p_test_cases_skipped, total_tc        , "test case", "skipped" );
        print_result( output, tr.p_test_cases_aborted, total_tc        , "test case", "aborted" );
    }
    output << "\n";
}

//____________________________________________________________________________//

void
compiler_log_formatter::test_unit_skipped( std::ostream& output, test_unit const& tu, const_string reason )
{
    BOOST_TEST_SCOPE_SETCOLOR( output, term_attr::BRIGHT, term_color::YELLOW );

    print_prefix( output, tu.p_file_name, tu.p_line_num );

    output  << "Test " << tu.p_type_name << " \"" << tu.full_name() << "\"" << " is skipped because " << reason << std::endl;
}

//____________________________________________________________________________//

void
compiler_log_formatter::log_exception_start( std::ostream& output, log_checkpoint_data const& checkpoint_data, execution_exception const& ex )
{
    execution_exception::location const& loc = ex.where();

    print_prefix( output, loc.m_file_name, loc.m_line_num );

    {
        BOOST_TEST_SCOPE_SETCOLOR( output, term_attr::BLINK, term_color::RED );

        output << "fatal error: in \"" << (loc.m_function.is_empty() ? test_phase_identifier() : loc.m_function ) << "\": "
               << ex.what();
    }

    if( !checkpoint_data.m_file_name.is_empty() ) {
        output << '\n';
        print_prefix( output, checkpoint_data.m_file_name, checkpoint_data.m_line_num );

        BOOST_TEST_SCOPE_SETCOLOR( output, term_attr::BRIGHT, term_color::CYAN );

        output << "last checkpoint";
        if( !checkpoint_data.m_message.empty() )
            output << ": " << checkpoint_data.m_message;
    }
}

//____________________________________________________________________________//

void
compiler_log_formatter::log_exception_finish( std::ostream& output )
{
    output << std::endl;
}

//____________________________________________________________________________//

void
compiler_log_formatter::log_entry_start( std::ostream& output, log_entry_data const& entry_data, log_entry_types let )
{
    switch( let ) {
        case BOOST_UTL_ET_INFO:
            print_prefix( output, entry_data.m_file_name, entry_data.m_line_num );
            if( runtime_config::color_output() )
                output << setcolor( term_attr::BRIGHT, term_color::GREEN );
            output << "info: ";
            break;
        case BOOST_UTL_ET_MESSAGE:
            if( runtime_config::color_output() )
                output << setcolor( term_attr::BRIGHT, term_color::CYAN );
            break;
        case BOOST_UTL_ET_WARNING:
            print_prefix( output, entry_data.m_file_name, entry_data.m_line_num );
            if( runtime_config::color_output() )
                output << setcolor( term_attr::BRIGHT, term_color::YELLOW );
            output << "warning: in \"" << test_phase_identifier() << "\": ";
            break;
        case BOOST_UTL_ET_ERROR:
            print_prefix( output, entry_data.m_file_name, entry_data.m_line_num );
            if( runtime_config::color_output() )
                output << setcolor( term_attr::BRIGHT, term_color::RED );
            output << "error: in \"" << test_phase_identifier() << "\": ";
            break;
        case BOOST_UTL_ET_FATAL_ERROR:
            print_prefix( output, entry_data.m_file_name, entry_data.m_line_num );
            if( runtime_config::color_output() )
                output << setcolor( term_attr::BLINK, term_color::RED );
            output << "fatal error: in \"" << test_phase_identifier() << "\": ";
            break;
    }
}

//____________________________________________________________________________//

void
compiler_log_formatter::log_entry_value( std::ostream& output, const_string value )
{
    output << value;
}

//____________________________________________________________________________//

void
compiler_log_formatter::log_entry_value( std::ostream& output, lazy_ostream const& value )
{
    output << value;
}

//____________________________________________________________________________//

void
compiler_log_formatter::log_entry_finish( std::ostream& output )
{
    if( runtime_config::color_output() )
        output << setcolor();

    output << std::endl;
}


//____________________________________________________________________________//

void
compiler_log_formatter::print_prefix( std::ostream& output, const_string file_name, std::size_t line_num )
{
    if( !file_name.empty() )
    {
#ifdef __APPLE_CC__
        // Xcode-compatible logging format, idea by Richard Dingwall at
        // <http://richarddingwall.name/2008/06/01/using-the-boost-unit-test-framework-with-xcode-3/>.
        output << file_name << ':' << line_num << ": ";
#else
        output << file_name << '(' << line_num << "): ";
#endif
    }
}

//____________________________________________________________________________//

void
compiler_log_formatter::entry_context_start( std::ostream& output, log_level l )
{
    output << (l == log_successful_tests ? "\nAssertion" : "\nFailure" ) << " occurred in a following context:";
}

//____________________________________________________________________________//

void
compiler_log_formatter::entry_context_finish( std::ostream& output )
{
    output.flush();
}

//____________________________________________________________________________//

void
compiler_log_formatter::log_entry_context( std::ostream& output, const_string context_descr )
{
    output << "\n    " << context_descr;
}

//____________________________________________________________________________//

} // namespace output
} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_COMPILER_LOG_FORMATTER_IPP_020105GER
