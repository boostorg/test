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
//  Description : contains xml log formatter definition
// ***************************************************************************

#ifndef BOOST_TEST_XML_LOG_FORMATTER_020105GER
#define BOOST_TEST_XML_LOG_FORMATTER_020105GER

// Boost.Test
#include <boost/test/unit_test_log_formatter.hpp>

// STL
#include <cstddef> // std::size_t

#include <boost/test/detail/suppress_warnings.hpp>

namespace boost {

namespace unit_test {

namespace output {

// ************************************************************************** //
// **************               xml_log_formatter              ************** //
// ************************************************************************** //

class xml_log_formatter : public unit_test_log_formatter {
public:
    xml_log_formatter();

    void    start_log( std::ostream&, bool log_build_info );
    void    log_header( std::ostream&, counter_t test_cases_amount );
    void    finish_log( std::ostream& );

    void    start_test_case( test_case const& tc );
    void    test_case_enter( std::ostream&, test_case const& tc );
    void    test_case_exit( std::ostream&, test_case const& tc, long testing_time_in_mks );

    void    log_exception( std::ostream&, log_checkpoint_data const&, const_string explanation );
    void    begin_log_entry( std::ostream&, log_entry_data const&, log_entry_types let );

    void    log_entry_value( std::ostream&, const_string value );
    void    end_log_entry( std::ostream& );

private:
    void    print_indent( std::ostream& );

    // Data members
    std::size_t     m_indent;
    const_string    m_curr_tag;
    const_string    m_curr_test_case_name;
};

} // namespace output

} // namespace unit_test

} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.1  2005/02/01 08:59:39  rogeeff
//  supplied_log_formatters split
//  change formatters interface to simplify result interface
//
// ***************************************************************************

#endif // BOOST_TEST_XML_LOG_FORMATTER_020105GER
