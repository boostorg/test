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
//  Description : implements compiler like log formatter
// ***************************************************************************

#ifndef BOOST_TEST_COMPILER_LOG_FORMATTER_IPP_020105GER
#define BOOST_TEST_COMPILER_LOG_FORMATTER_IPP_020105GER

// Boost.Test
#include <boost/test/output/compiler_log_formatter.hpp>
#include <boost/test/unit_test_suite.hpp>

// Boost
#include <boost/version.hpp>

// STL
#include <iostream>

namespace boost {

namespace unit_test {

namespace output {

// ************************************************************************** //
// **************            compiler_log_formatter            ************** //
// ************************************************************************** //

void
compiler_log_formatter::start_log( std::ostream& output, bool log_build_info )
{
    if( log_build_info )
        output  << "Platform: " << BOOST_PLATFORM            << '\n'
                << "Compiler: " << BOOST_COMPILER            << '\n'
                << "STL     : " << BOOST_STDLIB              << '\n'
                << "Boost   : " << BOOST_VERSION/100000      << "."
                                << BOOST_VERSION/100 % 1000  << "."
                                << BOOST_VERSION % 100       << '\n';
}

//____________________________________________________________________________//

void
compiler_log_formatter::log_header( std::ostream& output, counter_t test_cases_amount )
{
    output  << "Running " << test_cases_amount << " test "
            << (test_cases_amount > 1 ? "cases" : "case") << "...\n";
}

//____________________________________________________________________________//

void
compiler_log_formatter::finish_log( std::ostream& /* output */ )
{
}

//____________________________________________________________________________//

void
compiler_log_formatter::start_test_case( test_case const& tc )
{
    m_curr_test_case_name = tc.p_name;
}

//____________________________________________________________________________//

void
compiler_log_formatter::test_case_enter( std::ostream& output, test_case const& tc )
{
    output  << "Entering test " << ( tc.p_type ? "case" : "suite" ) << " \"" << m_curr_test_case_name << "\"";
}

//____________________________________________________________________________//

void
compiler_log_formatter::test_case_exit( std::ostream& output, test_case const& tc, long testing_time_in_mks )
{
    output << "Leaving test " << ( tc.p_type ? "case" : "suite" ) << " \"" << tc.p_name << "\"";
    if( testing_time_in_mks > 0 ) {
        output << "; testing time: ";
        if( testing_time_in_mks % 1000 == 0 )
            output << testing_time_in_mks/1000 << "ms";
        else
            output << testing_time_in_mks << "mks";
    }

    // we should've probably restored m_curr_test_case_name here, 
    // but we will reset it anyway next time in test_case_enter
}

//____________________________________________________________________________//

void
compiler_log_formatter::log_exception( std::ostream& output, log_checkpoint_data const& checkpoint_data,
                                       const_string explanation )
{
    output << "Exception in \"" << m_curr_test_case_name << "\": " << explanation;

    if( !checkpoint_data.m_message.empty() ) {
        output << '\n';
        print_prefix( output, checkpoint_data.m_file, checkpoint_data.m_line );
        output << "last checkpoint: " << checkpoint_data.m_message;
    }
}

//____________________________________________________________________________//

void
compiler_log_formatter::begin_log_entry( std::ostream& output, log_entry_data const& entry_data, log_entry_types let )
{
    switch( let ) {
        case BOOST_UTL_ET_INFO:
            print_prefix( output, entry_data.m_file, entry_data.m_line );
            output << "info: ";
            break;
        case BOOST_UTL_ET_MESSAGE:
            break;
        case BOOST_UTL_ET_WARNING:
            print_prefix( output, entry_data.m_file, entry_data.m_line );
            output << "warning in \"" << m_curr_test_case_name << "\": ";
            break;
        case BOOST_UTL_ET_ERROR:
            print_prefix( output, entry_data.m_file, entry_data.m_line );
            output << "error in \"" << m_curr_test_case_name << "\": ";
            break;
        case BOOST_UTL_ET_FATAL_ERROR:
            print_prefix( output, entry_data.m_file, entry_data.m_line );
            output << "fatal error in \"" << m_curr_test_case_name << "\": ";
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
compiler_log_formatter::end_log_entry( std::ostream& /* output */ )
{
}

//____________________________________________________________________________//

void
compiler_log_formatter::print_prefix( std::ostream& output, const_string file, std::size_t line )
{
    output << file << '(' << line << "): ";
}

//____________________________________________________________________________//

} // namespace ouptut

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.1  2005/02/01 08:59:38  rogeeff
//  supplied_log_formatters split
//  change formatters interface to simplify result interface
//
// ***************************************************************************

#endif // BOOST_TEST_COMPILER_LOG_FORMATTER_IPP_020105GER
