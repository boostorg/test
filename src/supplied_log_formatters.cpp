//  (C) Copyright Gennadiy Rozental 2001-2003.
//  See accompanying license for terms and conditions of use.

//  See http://www.boost.org for updates, documentation, and revision history.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : implemets Unit Test Log formatters
// ***************************************************************************

// Boost.Test
#include <boost/test/detail/supplied_log_formatters.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test_result.hpp>

// BOOST
#include <boost/version.hpp>

// STL
#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>

namespace boost {

namespace unit_test_framework {

namespace detail {

// ************************************************************************** //
// **************           msvc65_like_log_formatter          ************** //
// ************************************************************************** //

msvc65_like_log_formatter::msvc65_like_log_formatter( unit_test_log const& log ) 
: unit_test_log_formatter( log )
{}

//____________________________________________________________________________//

void
msvc65_like_log_formatter::start_log( std::ostream& output, bool log_build_info )
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
msvc65_like_log_formatter::log_header( std::ostream& output, unit_test_counter test_cases_amount )
{
    output  << "Running " << test_cases_amount << " test "
        << (test_cases_amount > 1 ? "cases" : "case") << "...\n";
}

//____________________________________________________________________________//

void
msvc65_like_log_formatter::finish_log( std::ostream& /* output */ )
{
}

//____________________________________________________________________________//

void
msvc65_like_log_formatter::track_test_case_scope( std::ostream& output, test_case const& tc, bool in_out )
{
    output << (in_out ? "Entering" : "Leaving")
        << " test " << ( tc.p_type ? "case" : "suite" )
        << " \"" << tc.p_name.get() << "\"";
}

//____________________________________________________________________________//

void
msvc65_like_log_formatter::log_exception( std::ostream& output, std::string const& test_case_name, c_string_literal explanation )
{
    output << "Exception in \"" << test_case_name << "\": " << explanation;

    log_checkpoint_data const& chpd = checkpoint_data();

    if( !chpd.m_message.empty() ) {
        output << '\n';
        print_prefix( output, chpd.m_file, chpd.m_line );
        output << "last checkpoint: " << chpd.m_message;
    }
}

//____________________________________________________________________________//

void
msvc65_like_log_formatter::begin_log_entry( std::ostream& output, log_entry_types let )
{
    switch( let ) {
        case BOOST_UTL_ET_INFO:
            print_prefix( output, entry_data().m_file, entry_data().m_line );
            output << "info: ";
            break;
        case BOOST_UTL_ET_MESSAGE:
            break;
        case BOOST_UTL_ET_WARNING:
            print_prefix( output, entry_data().m_file, entry_data().m_line );
            output << "warning in \"" << unit_test_result::instance().test_case_name() << "\": ";
            break;
        case BOOST_UTL_ET_ERROR:
            print_prefix( output, entry_data().m_file, entry_data().m_line );
            output << "error in \"" << unit_test_result::instance().test_case_name() << "\": ";
            break;
        case BOOST_UTL_ET_FATAL_ERROR:
            print_prefix( output, entry_data().m_file, entry_data().m_line );
            output << "fatal error in \"" << unit_test_result::instance().test_case_name() << "\": ";
            break;
    }
}

//____________________________________________________________________________//

void
msvc65_like_log_formatter::log_entry_value( std::ostream& output, std::string const& value )
{
    output << value;
}

//____________________________________________________________________________//

void
msvc65_like_log_formatter::end_log_entry( std::ostream& /* output */ )
{
}

//____________________________________________________________________________//

void
msvc65_like_log_formatter::print_prefix( std::ostream& output, std::string const& file, std::size_t line )
{
        output << file << '(' << line << "): ";
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************               xml_log_formatter              ************** //
// ************************************************************************** //

xml_log_formatter::xml_log_formatter( unit_test_log const& log ) 
: unit_test_log_formatter( log ), m_indent( 0 ), m_curr_tag( c_string_literal() )
{
}

//____________________________________________________________________________//

void
xml_log_formatter::start_log( std::ostream& output, bool log_build_info )
{
    output  << "<TestLog";

    if( log_build_info )
        output  << " platform=\"" << BOOST_PLATFORM            << '\"'
        << " compiler=\"" << BOOST_COMPILER            << '\"'
        << " stl=\""      << BOOST_STDLIB              << '\"'
        << " boost=\""    << BOOST_VERSION/100000      << "."
        << BOOST_VERSION/100 % 1000  << "."
        << BOOST_VERSION % 100       << '\"';

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
xml_log_formatter::track_test_case_scope( std::ostream& output, test_case const& tc, bool in_out )
{
    if( !in_out )
        m_indent -= 2;

    print_indent( output );

    output << (in_out ? "<" : "</")
        << ( tc.p_type ? "TestCase" : "TestSuite" );

    if( in_out )
        output << " name=\"" << tc.p_name.get() << "\"";

    output << ">";

    if( in_out )
        m_indent += 2;
}

//____________________________________________________________________________//

void
xml_log_formatter::log_exception( std::ostream& output, std::string const& test_case_name, c_string_literal explanation )
{
    print_indent( output );

    output << "<Exception name=\"" << test_case_name << "\">\n";

    m_indent += 2;
    print_indent( output );

    output << explanation << '\n';
    print_indent( output );

    log_checkpoint_data const& chpd = checkpoint_data();

    if( !chpd.m_message.empty() ) {
        output << "<LastCheckpoint file=\"" << chpd.m_file << "\""
            << " line=\"" << chpd.m_line << "\">\n";

        m_indent += 2;
        print_indent( output );

        output << chpd.m_message << "\n";

        m_indent -= 2;
        print_indent( output );

        output << "</LastCheckpoint>\n";

        m_indent -= 2;
        print_indent( output );
    }

    output << "</Exception>";
}

//____________________________________________________________________________//

void
xml_log_formatter::begin_log_entry( std::ostream& output, log_entry_types let )
{
    static c_string_literal const xml_tags[] = { "Info", "Message", "Warning", "Error", "FatalError" };

    print_indent( output );

    m_curr_tag = xml_tags[let];
    output << '<' << m_curr_tag
        << " file=\"" << entry_data().m_file<< '\"'
        << " line=\"" << entry_data().m_line << '\"'
        << ">\n";

    m_indent += 2;
    print_indent( output );
}

//____________________________________________________________________________//

void
xml_log_formatter::log_entry_value( std::ostream& output, std::string const& value )
{
    output << value;
}

//____________________________________________________________________________//

void
xml_log_formatter::end_log_entry( std::ostream& output )
{
    if( !m_curr_tag )
        return;

    output << '\n';

    m_indent -= 2;
    print_indent( output );

    output << "</" << m_curr_tag << ">";

    m_curr_tag = c_string_literal();
}

//____________________________________________________________________________//

void
xml_log_formatter::print_indent( std::ostream& output )
{
        output << std::setw( m_indent ) << "";
}

//____________________________________________________________________________//

} // namespace detail

} // namespace unit_test_framework

} // namespace boost

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.4  2003/10/27 07:13:32  rogeeff
//  licence update
//
//  Revision 1.3  2003/07/15 08:31:24  rogeeff
//  *** empty log message ***
//
//  Revision 1.2  2003/07/09 12:51:29  jmaurer
//  avoid "unused parameter" warnings with gcc
//
//  Revision 1.1  2003/07/02 09:11:24  rogeeff
//  move log formatter in public interface
//

// ***************************************************************************

// EOF
