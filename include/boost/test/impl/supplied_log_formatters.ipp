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
//  Description : implements Unit Test Log formatters
// ***************************************************************************

#ifndef BOOST_SUPPLIED_LOG_FORMATTERS_IPP_012205GER
#define BOOST_SUPPLIED_LOG_FORMATTERS_IPP_012205GER

// Boost.Test
#include <boost/test/detail/supplied_log_formatters.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test_result.hpp>
#include <boost/test/unit_test_log.hpp>

#include <boost/test/utils/xml_printer.hpp>

// BOOST
#include <boost/version.hpp>

// STL
#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>

namespace boost {

namespace unit_test {

namespace ut_detail {

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
compiler_log_formatter::log_header( std::ostream& output, unit_test_counter test_cases_amount )
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
compiler_log_formatter::track_test_case_enter( std::ostream& output, test_case const& tc )
{
    output  << "Entering test " << ( tc.p_type ? "case" : "suite" ) << " \"" << tc.p_name << "\"";
}

//____________________________________________________________________________//

void
compiler_log_formatter::track_test_case_exit( std::ostream& output, test_case const& tc, long testing_time_in_mks )
{
    output << "Leaving test " << ( tc.p_type ? "case" : "suite" ) << " \"" << tc.p_name << "\"";
    if( testing_time_in_mks > 0 ) {
        output << "; testing time: ";
        if( testing_time_in_mks % 1000 == 0 )
            output << testing_time_in_mks/1000 << "ms";
        else
            output << testing_time_in_mks << "ms";
    }
}

//____________________________________________________________________________//

void
compiler_log_formatter::log_exception( std::ostream& output, log_checkpoint_data const& checkpoint_data ,
                                       const_string test_case_name, const_string explanation )
{
    output << "Exception in \"" << test_case_name << "\": " << explanation;

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
            output << "warning in \"" << unit_test_result::instance().test_case_name() << "\": ";
            break;
        case BOOST_UTL_ET_ERROR:
            print_prefix( output, entry_data.m_file, entry_data.m_line );
            output << "error in \"" << unit_test_result::instance().test_case_name() << "\": ";
            break;
        case BOOST_UTL_ET_FATAL_ERROR:
            print_prefix( output, entry_data.m_file, entry_data.m_line );
            output << "fatal error in \"" << unit_test_result::instance().test_case_name() << "\": ";
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

// ************************************************************************** //
// **************               xml_log_formatter              ************** //
// ************************************************************************** //

xml_log_formatter::xml_log_formatter() 
: m_indent( 0 )
{
}

//____________________________________________________________________________//

void
xml_log_formatter::start_log( std::ostream& output, bool log_build_info )
{
    output  << "<TestLog";

    if( log_build_info ) {
        output  << " platform"  << attr_value() << BOOST_PLATFORM
                << " compiler"  << attr_value() << BOOST_COMPILER
                << " stl"       << attr_value() << BOOST_STDLIB
                << " boost=\""  << BOOST_VERSION/100000     << "."
                                << BOOST_VERSION/100 % 1000 << "."
                                << BOOST_VERSION % 100      << '\"';
    }

    output  << ">\n";
}

//____________________________________________________________________________//

void
xml_log_formatter::log_header( std::ostream& /* output */, unit_test_counter /* test_cases_amount */ )
{
}

//____________________________________________________________________________//

void
xml_log_formatter::finish_log( std::ostream& output )
{
    output  << "</TestLog>\n";
}

//____________________________________________________________________________//

void
xml_log_formatter::track_test_case_enter( std::ostream& output, test_case const& tc )
{
    print_indent( output );

    output << "<" << ( tc.p_type ? "TestCase" : "TestSuite" )
           << " name" << attr_value() << tc.p_name
           << ">";

    m_indent += 2;
}

//____________________________________________________________________________//

void
xml_log_formatter::track_test_case_exit( std::ostream& output, test_case const& tc, long testing_time_in_mks )
{
    m_indent -= 2;

    print_indent( output );

    output << "</" << ( tc.p_type ? "TestCase" : "TestSuite" )
           << " testing_time" << attr_value() << testing_time_in_mks
           << ">";
}

//____________________________________________________________________________//

void
xml_log_formatter::log_exception( std::ostream& output, log_checkpoint_data const& checkpoint_data, const_string test_case_name, const_string explanation )
{
    print_indent( output );
    output << "<Exception name" << attr_value() << test_case_name << ">\n";
    
    m_indent += 2;

    print_indent( output );
    output << pcdata() << explanation << '\n';

    if( !checkpoint_data.m_message.empty() ) {
        print_indent( output );
        output << "<LastCheckpoint file" << attr_value() << checkpoint_data.m_file
               << " line"                << attr_value() << checkpoint_data.m_line
               << ">\n";

        m_indent += 2;

        print_indent( output );
        output << pcdata() << checkpoint_data.m_message << "\n";

        m_indent -= 2;

        print_indent( output );
        output << "</LastCheckpoint>\n";
    }

    m_indent -= 2;
    print_indent( output );

    output << "</Exception>";
}

//____________________________________________________________________________//

void
xml_log_formatter::begin_log_entry( std::ostream& output, log_entry_data const& entry_data, log_entry_types let )
{
    static literal_string xml_tags[] = { "Info", "Message", "Warning", "Error", "FatalError" };

    print_indent( output );

    m_curr_tag = xml_tags[let];
    output << '<' << m_curr_tag
           << " file" << attr_value() << entry_data.m_file
           << " line" << attr_value() << entry_data.m_line
           << ">\n";

    m_indent += 2;
    print_indent( output );
}

//____________________________________________________________________________//

void
xml_log_formatter::log_entry_value( std::ostream& output, const_string value )
{
    output << pcdata() << value;
}

//____________________________________________________________________________//

void
xml_log_formatter::end_log_entry( std::ostream& output )
{
    if( m_curr_tag.is_empty() )
        return;

    output << '\n';

    m_indent -= 2;
    print_indent( output );

    output << "</" << m_curr_tag << ">";

    m_curr_tag.clear();
}

//____________________________________________________________________________//

void
xml_log_formatter::print_indent( std::ostream& output )
{
    output << std::setw( m_indent ) << "";
}

//____________________________________________________________________________//

} // namespace ut_detail

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.1  2005/01/22 19:22:12  rogeeff
//  implementation moved into headers section to eliminate dependency of included/minimal component on src directory
//
//  Revision 1.15  2005/01/21 07:23:49  rogeeff
//  added automatic test case run timing
//
//  Revision 1.13  2005/01/18 08:29:59  rogeeff
//  unit_test_log rework:
//     eliminated need for ::instance()
//     eliminated need for << end and ...END macro
//     straitend interface between log and formatters
//     change compiler like formatter name
//     minimized unit_test_log interface and reworked to use explicit calls
//
//  Revision 1.12  2004/07/20 17:03:55  dgregor
//  Documentation update from Jon T. Pedant
//
//  Revision 1.11  2004/07/19 12:09:25  rogeeff
//  added proper encoded of XML PCDATA
//
//  Revision 1.10  2004/06/07 07:34:22  rogeeff
//  detail namespace renamed
//
//  Revision 1.9  2004/05/21 06:26:09  rogeeff
//  licence update
//
//  Revision 1.8  2004/05/13 09:04:43  rogeeff
//  added fixed_mapping
//
//  Revision 1.7  2004/05/11 11:04:44  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//  namespace names shortened
//
//  Revision 1.6  2003/12/01 00:42:37  rogeeff
//  prerelease cleaning
//
// ***************************************************************************

#endif // BOOST_SUPPLIED_LOG_FORMATTERS_IPP_012205GER
