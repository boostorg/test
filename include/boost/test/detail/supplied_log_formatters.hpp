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
//  Description : contains log formatters supplied by the framework definitions 
// ***************************************************************************

#ifndef BOOST_TEST_SUPPLIED_LOG_FORMATTERS_HPP_071894GER
#define BOOST_TEST_SUPPLIED_LOG_FORMATTERS_HPP_071894GER

// Boost.Test
#include <boost/test/unit_test_log_formatter.hpp>

// BOOST
#include <boost/config.hpp>

// STL
#include <cstddef>

#include <boost/test/detail/suppress_warnings.hpp>

namespace boost {

namespace unit_test {

namespace ut_detail {

// ************************************************************************** //
// **************           compiler_log_formatter          ************** //
// ************************************************************************** //

class compiler_log_formatter : public unit_test_log_formatter {
public:
    void    start_log( std::ostream&, bool log_build_info );
    void    log_header( std::ostream&, unit_test_counter test_cases_amount );
    void    finish_log( std::ostream& );

    void    track_test_case_enter( std::ostream&, test_case const& tc );
    void    track_test_case_exit( std::ostream&, test_case const& tc, long testing_time_in_mks );

    void    log_exception( std::ostream&, log_checkpoint_data const&, const_string test_case_name, const_string explanation );
    void    begin_log_entry( std::ostream&, log_entry_data const&, log_entry_types let );

    void    log_entry_value( std::ostream&, const_string value );
    void    end_log_entry( std::ostream& );

protected:
    virtual void    print_prefix( std::ostream&, const_string file, std::size_t line );
};

// ************************************************************************** //
// **************               xml_log_formatter              ************** //
// ************************************************************************** //

class xml_log_formatter : public unit_test_log_formatter {
public:
    xml_log_formatter();

    void    start_log( std::ostream&, bool log_build_info );
    void    log_header( std::ostream&, unit_test_counter test_cases_amount );
    void    finish_log( std::ostream& );

    void    track_test_case_enter( std::ostream&, test_case const& tc );
    void    track_test_case_exit( std::ostream&, test_case const& tc, long testing_time_in_mks );

    void    log_exception( std::ostream&, log_checkpoint_data const&, const_string test_case_name, const_string explanation );
    void    begin_log_entry( std::ostream&, log_entry_data const&, log_entry_types let );

    void    log_entry_value( std::ostream&, const_string value );
    void    end_log_entry( std::ostream& );

private:
    void    print_indent( std::ostream& );

    // Data members
    std::size_t     m_indent;
    const_string    m_curr_tag;
};

} // namespace ut_detail

} // namespace unit_test

} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.11  2005/01/22 19:22:12  rogeeff
//  implementation moved into headers section to eliminate dependency of included/minimal component on src directory
//
//  Revision 1.10  2005/01/21 07:30:45  rogeeff
//  to log testing time log formatter interfaces changed
//
//  Revision 1.9  2005/01/18 08:26:12  rogeeff
//  unit_test_log rework:
//     eliminated need for ::instance()
//     eliminated need for << end and ...END macro
//     straitend interface between log and formatters
//     change compiler like formatter name
//     minimized unit_test_log interface and reworked to use explicit calls
//
//  Revision 1.8  2004/07/19 12:22:49  rogeeff
//  guard rename
//  suppress warnings
//
//  Revision 1.7  2004/06/07 07:33:49  rogeeff
//  detail namespace renamed
//
//  Revision 1.6  2004/05/21 06:19:35  rogeeff
//  licence update
//
//  Revision 1.5  2004/05/11 11:00:53  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//
//  Revision 1.4  2003/12/01 00:41:56  rogeeff
//  prerelease cleaning
//
// ***************************************************************************

#endif // BOOST_TEST_SUPPLIED_LOG_FORMATTERS_HPP_071894GER
