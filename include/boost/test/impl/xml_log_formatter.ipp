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
//  Description : implements Unit Test Log formatters
// ***************************************************************************

#ifndef BOOST_TEST_XML_LOG_FORMATTER_IPP_020105GER
#define BOOST_TEST_XML_LOG_FORMATTER_IPP_020105GER

// Boost.Test
#include <boost/test/output/xml_log_formatter.hpp>
#include <boost/test/unit_test_suite.hpp>

#include <boost/test/utils/xml_printer.hpp>

// Boost
#include <boost/version.hpp>

// STL
#include <iostream>
#include <iomanip>

namespace boost {

namespace unit_test {

namespace output {

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
xml_log_formatter::log_header( std::ostream& /* output */, counter_t /* test_cases_amount */ )
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
xml_log_formatter::start_test_case( test_case const& tc )
{
    m_curr_test_case_name = tc.p_name;
}

//____________________________________________________________________________//

void
xml_log_formatter::test_case_enter( std::ostream& output, test_case const& tc )
{
    print_indent( output );

    output << "<" << ( tc.p_type ? "TestCase" : "TestSuite" )
           << " name" << attr_value() << m_curr_test_case_name << ">";

    m_indent += 2;
}

//____________________________________________________________________________//

void
xml_log_formatter::test_case_exit( std::ostream& output, test_case const& tc, long testing_time_in_mks )
{
    print_indent( output );

    output << "<TestingTime>" << testing_time_in_mks << "</TestingTime>";

    m_indent -= 2;

    print_indent( output );

    output << "</" << ( tc.p_type ? "TestCase" : "TestSuite" ) << ">";
}

//____________________________________________________________________________//

void
xml_log_formatter::log_exception( std::ostream& output, log_checkpoint_data const& checkpoint_data, const_string explanation )
{
    print_indent( output );
    output << "<Exception name" << attr_value() << m_curr_test_case_name << ">\n";
    
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

} // namespace output

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

#endif // BOOST_TEST_XML_LOG_FORMATTER_IPP_020105GER
