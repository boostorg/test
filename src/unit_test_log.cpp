//  (C) Copyright Gennadiy Rozental 2001-2002.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org for most recent version including documentation.

// LOCAL
#include <boost/test/unit_test_log.hpp>
#include <boost/test/unit_test_result.hpp>
#include <boost/test/unit_test_suite.hpp>

// BOOST
#include <boost/scoped_ptr.hpp>
#include <boost/progress.hpp>
#include <boost/version.hpp>

// STL
#include <iostream>
#include <cstring>

# ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using :: strcmp; }
# endif

namespace boost {

namespace unit_test_framework {

// ************************************************************************** //
// **************                log manipulators              ************** //
// ************************************************************************** //

struct unit_test_log::Impl {
    // Constructor
    Impl() : m_stream( &std::cout ) {}

    // log data
    std::ostream*   m_stream;
    report_level    m_threshold_level;
    unit_test_counter m_expected_total_test_cases_amount;
    boost::scoped_ptr<boost::progress_display> m_progress_display;

    // entry data
    char const*     m_entry_file;
    unsigned int    m_entry_line;
    report_level    m_entry_level;
    bool            m_entry_in_progress;
    bool            m_entry_has_value;

    // checkpoint data
    char const*     m_checkpoint_file;
    unsigned int    m_checkpoint_line;
    std::string     m_checkpoint_message;

    // helper functions
    std::ostream&   stream() { return *m_stream; }

    void            clear_entry_data() {
        m_entry_file            = NULL;
        m_entry_line            = 0;
        m_entry_level           = report_nothing;
        m_entry_in_progress     = false;
        m_entry_has_value       = false;
    }

    void            clear_checkpoint_data() {
        m_checkpoint_file       = NULL;
        m_checkpoint_line       = 0;
        m_checkpoint_message    = "";
    }

    void            flush_entry()       { stream() << std::endl; }

    void            print_prefix() {
        stream() << m_entry_file << '(' << m_entry_line << ") : ";
    }
};

//____________________________________________________________________________//

unit_test_log&
unit_test_log::instance() {
    static unit_test_log the_instance;

    return the_instance;
}

//____________________________________________________________________________//

unit_test_log::unit_test_log() : m_pimpl( new Impl() )
{
    m_pimpl->m_threshold_level = report_all_errors;
    m_pimpl->m_expected_total_test_cases_amount = 0;

    m_pimpl->clear_entry_data();
    m_pimpl->clear_checkpoint_data();
}

//____________________________________________________________________________//

unit_test_log::~unit_test_log()
{
    delete m_pimpl;
}

//____________________________________________________________________________//

void
unit_test_log::set_log_stream( std::ostream& str )
{
    m_pimpl->m_stream = &str;
}

//____________________________________________________________________________//

void
unit_test_log::set_log_threshold_level( report_level lev )
{
    m_pimpl->m_threshold_level = lev;
}

//____________________________________________________________________________//

void
unit_test_log::clear_checkpoint()
{
    m_pimpl->clear_checkpoint_data();
}

//____________________________________________________________________________//

unit_test_log&
unit_test_log::operator<<( begin const& )
{
    if( m_pimpl->m_entry_in_progress )
        *this << end();

    m_pimpl->m_entry_in_progress = true;

    return *this;
}

//____________________________________________________________________________//

unit_test_log&
unit_test_log::operator<<( end const& )
{
    if( m_pimpl->m_entry_has_value ) {
        m_pimpl->flush_entry();
    }

    m_pimpl->clear_entry_data();

    return *this;
}

//____________________________________________________________________________//

unit_test_log&
unit_test_log::operator<<( file const& f )
{
    if( m_pimpl->m_entry_in_progress )
        m_pimpl->m_entry_file = f.m_file_name;

    return *this;
}

//____________________________________________________________________________//

unit_test_log&
unit_test_log::operator<<( line const& l )
{
    if( m_pimpl->m_entry_in_progress )
        m_pimpl->m_entry_line = l.m_line_num;

    return *this;
}

//____________________________________________________________________________//

unit_test_log&
unit_test_log::operator<<( level const& lev )
{
    if( m_pimpl->m_entry_in_progress )
        m_pimpl->m_entry_level = lev.m_level;

    return *this;
}

//____________________________________________________________________________//

unit_test_log&
unit_test_log::operator<<( checkpoint const& cp )
{
    if( m_pimpl->m_entry_in_progress ) {
        m_pimpl->m_checkpoint_message   = cp.m_message;
        m_pimpl->m_checkpoint_file      = m_pimpl->m_entry_file;
        m_pimpl->m_checkpoint_line      = m_pimpl->m_entry_line;
    }

    return *this;
}

//____________________________________________________________________________//

unit_test_log&
unit_test_log::operator<<( report_exception const& re )
{
    if( m_pimpl->m_entry_in_progress && m_pimpl->m_entry_level >= m_pimpl->m_threshold_level ) {
        m_pimpl->stream() << "Exception in \"" << unit_test_result::instance().test_case_name() 
                          << "\": " << re.m_what;
        m_pimpl->m_entry_has_value = true;

        if( !m_pimpl->m_checkpoint_message.empty() ) {
            m_pimpl->stream() << '\n' << m_pimpl->m_checkpoint_file << '(' << m_pimpl->m_checkpoint_line << ") : "
                << "last checkpoint: " << m_pimpl->m_checkpoint_message;
        }
    }

    return *this;
}

//____________________________________________________________________________//

unit_test_log&
unit_test_log::operator<<( report_progress const& )
{
    if( m_pimpl->m_progress_display.get() != NULL )
        ++(*m_pimpl->m_progress_display);

    return *this;
}

//____________________________________________________________________________//

void
unit_test_log::set_log_threshold_level_by_name( char const* lev )
{
    struct my_pair {
        char const*     level_name;
        report_level    named_level;
    };

    static const my_pair named_level[] = {
        { "all"             , report_successful_tests },
        { "success"         , report_successful_tests },
        { "test_suite"      , report_test_suites },
        { "messages"        , report_messages },
        { "warnings"        , report_warnings },
        { "all_errors"      , report_all_errors },
        { "cpp_exceptions"  , report_cpp_exception_errors },
        { "system_errors"   , report_system_errors },
        { "fatal_errors"    , report_fatal_errors },
        { "progress"        , report_progress_only},
        { "nothing"         , report_nothing },
    };

    static int const map_size = sizeof(named_level)/sizeof(my_pair);

    for( int i =0; i < map_size; i++ ) {
        if( std::strcmp( named_level[i].level_name, lev ) == 0 ) {
            set_log_threshold_level( named_level[i].named_level );
            return;
        }
    }
}

//____________________________________________________________________________//

unit_test_log&
unit_test_log::operator<<( char const* value )
{
    if( m_pimpl->m_entry_in_progress && m_pimpl->m_entry_level >= m_pimpl->m_threshold_level ) {
        if( !m_pimpl->m_entry_has_value ) {
            switch( m_pimpl->m_entry_level ) {
            case report_successful_tests:
                m_pimpl->print_prefix();
                m_pimpl->stream() << "info: ";
                break;
            case report_test_suites:
            case report_messages:
                break;
            case report_warnings:
                m_pimpl->print_prefix();
                m_pimpl->stream() << "warning in \"" << unit_test_result::instance().test_case_name() << "\": ";
                break;
            case report_all_errors:
            case report_cpp_exception_errors:
            case report_system_errors:
                m_pimpl->print_prefix();
                m_pimpl->stream() << "error in \"" << unit_test_result::instance().test_case_name() << "\": ";
                break;
            case report_fatal_errors:
                m_pimpl->print_prefix();
                m_pimpl->stream() << "fatal error in \"" << unit_test_result::instance().test_case_name() << "\": ";
                break;
            case report_progress_only:
            case report_nothing:
                return *this;
            }
        }

        m_pimpl->stream() << value;
        m_pimpl->m_entry_has_value = true;
    }
    return *this;
}

//____________________________________________________________________________//

unit_test_log&
unit_test_log::operator<<( std::string const& value ) 
{
    return *this << value.c_str();
}

//____________________________________________________________________________//

void
unit_test_log::start( unit_test_counter test_cases_amount, bool print_build_info )
{
    m_pimpl->m_expected_total_test_cases_amount = test_cases_amount;

    if( m_pimpl->m_threshold_level != report_nothing && test_cases_amount > 0 )
        m_pimpl->stream() << "Running " << test_cases_amount << " test " 
                          << (test_cases_amount > 1 ? "cases" : "case") << "...\n";

    if( print_build_info && m_pimpl->m_threshold_level <= report_test_suites ) {
        if( m_pimpl->m_threshold_level <= report_test_suites ) {
            m_pimpl->stream() << " Platform: " << BOOST_PLATFORM            << '\n'
                              << " Compiler: " << BOOST_COMPILER            << '\n'
                              << " STL     : " << BOOST_STDLIB              << '\n'
                              << " Boost   : " << BOOST_VERSION/100000      << "." 
                                               << BOOST_VERSION/100 % 1000  << "." 
                                               << BOOST_VERSION % 100       << '\n';
        }

        
    }

    m_pimpl->m_progress_display.reset( m_pimpl->m_threshold_level == report_progress_only         ?
        new boost::progress_display( m_pimpl->m_expected_total_test_cases_amount, m_pimpl->stream() ) :
        NULL );
}

//____________________________________________________________________________//

} // namespace unit_test_framework

} // namespace boost

//  Revision History
//   5 Oct 01  Initial version (Gennadiy Rozental)

// EOF
