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
//  Description : implemets Unit Test Log, Majority of implementation details
//  are hidden in this file with use of pimpl idiom.
// ***************************************************************************

#ifndef BOOST_UNIT_TEST_LOG_IPP_012205GER
#define BOOST_UNIT_TEST_LOG_IPP_012205GER

// Boost.Test
#include <boost/test/unit_test_log.hpp>
#include <boost/test/unit_test_log_formatter.hpp>
#include <boost/test/unit_test_result.hpp>

#include <boost/test/detail/unit_test_parameters.hpp>

#include <boost/test/utils/basic_cstring/compare.hpp>
#include <boost/test/utils/fixed_mapping.hpp>

#include <boost/test/output/compiler_log_formatter.hpp>
#include <boost/test/output/xml_log_formatter.hpp>

// Boost
#include <boost/scoped_ptr.hpp>
#include <boost/progress.hpp>

// STL
#include <algorithm>
#include <map>

# ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::strcmp; }
# endif

namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************             entry_value_collector            ************** //
// ************************************************************************** //

namespace ut_detail {

entry_value_collector
entry_value_collector::operator<<( const_string v )
{
    unit_test_log << v;

    m_last = false;

    entry_value_collector res;
    return res;
}

//____________________________________________________________________________//

entry_value_collector
entry_value_collector::operator<<( log::checkpoint const& cp )
{
    unit_test_log << cp;

    m_last = false;

    entry_value_collector res;
    return res;
}

//____________________________________________________________________________//

entry_value_collector::~entry_value_collector()
{
    if( m_last )
        unit_test_log << log::end();
}

//____________________________________________________________________________//

} // namespace ut_detail

// ************************************************************************** //
// **************                 unit_test_log                ************** //
// ************************************************************************** //

namespace { 

struct unit_test_log_impl {
    // Constructor
    unit_test_log_impl() : m_stream( &std::cout ) {}

    // log data
    std::ostream*       m_stream;
    log_level           m_threshold_level;
    boost::scoped_ptr<boost::progress_display> m_progress_display;
    boost::scoped_ptr<unit_test_log_formatter> m_log_formatter;

    // entry data
    bool                m_entry_in_progress;
    bool                m_entry_has_value;
    log_entry_data      m_entry_data;

    // checkpoint data
    log_checkpoint_data m_checkpoint_data;

    // helper functions
    std::ostream&       stream()            { return *m_stream; }
    void                flush_entry()       { stream() << std::endl; }
    void                clear_entry_data()
    {
        m_entry_data.clear();
        m_entry_in_progress = false;
        m_entry_has_value   = false;
    }
    void                set_checkpoint( log::checkpoint const& cp )
    {
        cp.m_message.assign_to( m_checkpoint_data.m_message );
        m_checkpoint_data.m_file    = m_entry_data.m_file;
        m_checkpoint_data.m_line    = m_entry_data.m_line;
    }
    void                clear_checkpoint()  { m_checkpoint_data.clear(); }
};

unit_test_log_impl& s_impl() { static unit_test_log_impl the_inst; return the_inst; }

} // local namespace

//____________________________________________________________________________//

unit_test_log_t::unit_test_log_t()
{
    s_impl().m_threshold_level = log_all_errors;

    s_impl().m_log_formatter.reset( new output::compiler_log_formatter );

    s_impl().clear_entry_data();
    s_impl().clear_checkpoint();
}

//____________________________________________________________________________//

unit_test_log_t&
unit_test_log_t::instance()
{
    static unit_test_log_t the_instance;

    return the_instance;
}

//____________________________________________________________________________//

void
unit_test_log_t::set_stream( std::ostream& str )
{
    if( s_impl().m_entry_in_progress )
        return;

    s_impl().m_stream = &str;
}

//____________________________________________________________________________//

void
unit_test_log_t::set_threshold_level( log_level lev )
{
    if( s_impl().m_entry_in_progress || lev == invalid_log_level )
        return;

    s_impl().m_threshold_level = lev;
}

//____________________________________________________________________________//

void
unit_test_log_t::set_threshold_level_by_name( const_string lev )
{
    static fixed_mapping<const_string,log_level> log_level_name(
        "all"           , log_successful_tests,
        "success"       , log_successful_tests,
        "test_suite"    , log_test_suites,
        "messages"      , log_messages,
        "warnings"      , log_warnings,
        "all_errors"    , log_all_errors,
        "cpp_exceptions", log_cpp_exception_errors,
        "system_errors" , log_system_errors,
        "fatal_errors"  , log_fatal_errors,
        "progress"      , log_progress_only,
        "nothing"       , log_nothing,

        invalid_log_level
    );

    if( s_impl().m_entry_in_progress )
        return;

    set_threshold_level( log_level_name[lev] );
}

//____________________________________________________________________________//

void
unit_test_log_t::test_case_enter( test_case const& tc )
{
    s_impl().m_log_formatter->start_test_case( tc );

    if( s_impl().m_threshold_level > log_test_suites )
        return;

    *this << log::begin();

    s_impl().m_log_formatter->test_case_enter( s_impl().stream(), tc );
    s_impl().m_entry_has_value = true;

    *this << log::end();
}

//____________________________________________________________________________//

void
unit_test_log_t::test_case_exit( test_case const& tc, long testing_time_in_mks )
{
    if( s_impl().m_threshold_level > log_test_suites )
        return;

    s_impl().clear_checkpoint();

    *this << log::begin();

    s_impl().m_log_formatter->test_case_exit( s_impl().stream(), tc, testing_time_in_mks );
    s_impl().m_entry_has_value = true;

    *this << log::end();
}

//____________________________________________________________________________//

unit_test_log_t&
unit_test_log_t::operator<<( log::begin const& )
{
    if( s_impl().m_entry_in_progress )
        *this << log::end();

    s_impl().m_entry_in_progress = true;

    return *this;
}

//____________________________________________________________________________//

unit_test_log_t&
unit_test_log_t::operator<<( log::end const& )
{
    if( s_impl().m_entry_has_value ) {
        s_impl().m_log_formatter->end_log_entry( s_impl().stream() );
        s_impl().flush_entry();
    }

    s_impl().clear_entry_data();

    return *this;
}

//____________________________________________________________________________//

char
set_unix_slash( char in )
{
    return in == '\\' ? '/' : in;
}

unit_test_log_t&
unit_test_log_t::operator<<( log::file const& f )
{
    if( s_impl().m_entry_in_progress ) {
        f.m_file_name.assign_to( s_impl().m_entry_data.m_file );

        // normalize file name
        std::transform( s_impl().m_entry_data.m_file.begin(), s_impl().m_entry_data.m_file.end(), 
                        s_impl().m_entry_data.m_file.begin(),
                        &set_unix_slash );
    }

    return *this;
}

//____________________________________________________________________________//

unit_test_log_t&
unit_test_log_t::operator<<( log::line const& l )
{
    if( s_impl().m_entry_in_progress )
        s_impl().m_entry_data.m_line = l.m_line_num;

    return *this;
}

//____________________________________________________________________________//

unit_test_log_t&
unit_test_log_t::operator<<( log::checkpoint const& cp )
{
    if( s_impl().m_entry_in_progress )
        s_impl().set_checkpoint( cp );

    return *this;
}

//____________________________________________________________________________//

unit_test_log_t&
unit_test_log_t::operator<<( log_level l )
{
    s_impl().m_entry_data.m_level = l;

    return *this;
}

//____________________________________________________________________________//

ut_detail::entry_value_collector
unit_test_log_t::operator()( log_level l )
{
    if( !s_impl().m_entry_in_progress )
        *this << log::begin();

    *this << l;

    ut_detail::entry_value_collector res;
    return res;
}

//____________________________________________________________________________//

void
unit_test_log_t::log_exception( log_level l, const_string what )
{
    *this << log::begin();

    if( l >= s_impl().m_threshold_level ) {
        s_impl().m_log_formatter->log_exception( s_impl().stream(), s_impl().m_checkpoint_data, what );
        s_impl().m_entry_has_value = true;
    }

    *this << log::end();
}

//____________________________________________________________________________//

void
unit_test_log_t::log_progress()
{
    *this << log::begin();

    if( s_impl().m_progress_display )
        ++(*s_impl().m_progress_display);

    *this << log::end();
}

//____________________________________________________________________________//

unit_test_log_t&
unit_test_log_t::operator<<( const_string value )
{
    if( s_impl().m_entry_in_progress && s_impl().m_entry_data.m_level >= s_impl().m_threshold_level && !value.empty() ) {
        if( !s_impl().m_entry_has_value ) {
            switch( s_impl().m_entry_data.m_level ) {
            case log_successful_tests:
                s_impl().m_log_formatter->begin_log_entry( s_impl().stream(), s_impl().m_entry_data,
                                                         unit_test_log_formatter::BOOST_UTL_ET_INFO );
                break;
            case log_messages:
                s_impl().m_log_formatter->begin_log_entry( s_impl().stream(), s_impl().m_entry_data,
                                                         unit_test_log_formatter::BOOST_UTL_ET_MESSAGE );
                break;
            case log_warnings:
                s_impl().m_log_formatter->begin_log_entry( s_impl().stream(), s_impl().m_entry_data,
                                                         unit_test_log_formatter::BOOST_UTL_ET_WARNING );
                break;
            case log_all_errors:
            case log_cpp_exception_errors:
            case log_system_errors:
                s_impl().m_log_formatter->begin_log_entry( s_impl().stream(), s_impl().m_entry_data,
                                                         unit_test_log_formatter::BOOST_UTL_ET_ERROR );
                break;
            case log_fatal_errors:
                s_impl().m_log_formatter->begin_log_entry( s_impl().stream(), s_impl().m_entry_data,
                                                         unit_test_log_formatter::BOOST_UTL_ET_FATAL_ERROR );
                break;
            case log_progress_only:
            case log_nothing:
            case log_test_suites:
            case invalid_log_level:
                return *this;
            }
        }

        s_impl().m_log_formatter->log_entry_value( s_impl().stream(), value );
        s_impl().m_entry_has_value = true;
    }
    
    return *this;
}

//____________________________________________________________________________//

void
unit_test_log_t::start( bool log_build_info )
{
    s_impl().m_log_formatter->start_log( s_impl().stream(), log_build_info );
}

//____________________________________________________________________________//

void
unit_test_log_t::header( counter_t test_cases_amount )
{
    if( s_impl().m_threshold_level != log_nothing && test_cases_amount > 0 )
        s_impl().m_log_formatter->log_header( s_impl().stream(), test_cases_amount );

    if( s_impl().m_threshold_level == log_progress_only )
        s_impl().m_progress_display.reset(
            new boost::progress_display( test_cases_amount, s_impl().stream() ) );
    else
        s_impl().m_progress_display.reset();
}

//____________________________________________________________________________//

void
unit_test_log_t::finish( counter_t test_cases_amount )
{
    if( test_cases_amount == 1 )
        log_progress();

    s_impl().m_log_formatter->finish_log( s_impl().stream() );
}

//____________________________________________________________________________//

void
unit_test_log_t::set_format( const_string log_format_name )
{
    if( s_impl().m_entry_in_progress )
        return;

    static fixed_mapping<const_string,output_format,case_ins_less<char const> > log_format(
        "HRF", HRF,
        "XML", XML,

        HRF
        );

    if( log_format[log_format_name] == HRF )
        set_formatter( new output::compiler_log_formatter );
    else
        set_formatter( new output::xml_log_formatter );
}

//____________________________________________________________________________//

void
unit_test_log_t::set_formatter( unit_test_log_formatter* the_formatter )
{
    s_impl().m_log_formatter.reset( the_formatter );
}

//____________________________________________________________________________//

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.5  2005/02/02 12:08:14  rogeeff
//  namespace log added for log manipulators
//
//  Revision 1.4  2005/02/01 08:59:38  rogeeff
//  supplied_log_formatters split
//  change formatters interface to simplify result interface
//
//  Revision 1.3  2005/02/01 06:40:07  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
//  Revision 1.2  2005/01/30 01:59:36  rogeeff
//  counter type renamed
//
//  Revision 1.1  2005/01/22 19:22:13  rogeeff
//  implementation moved into headers section to eliminate dependency of included/minimal component on src directory
//
//  Revision 1.27  2005/01/21 07:23:49  rogeeff
//  added automatic test case run timing
//
//  Revision 1.25  2005/01/18 08:30:08  rogeeff
//  unit_test_log rework:
//     eliminated need for ::instance()
//     eliminated need for << end and ...END macro
//     straitend interface between log and formatters
//     change compiler like formatter name
//     minimized unit_test_log interface and reworked to use explicit calls
//
// ***************************************************************************

#endif // BOOST_UNIT_TEST_LOG_IPP_012205GER
