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
//  Description : defines singleton class unit_test_log and all manipulators.
//  unit_test_log has output stream like interface. It's implementation is
//  completely hidden with pimple idiom
// ***************************************************************************

#ifndef BOOST_UNIT_TEST_LOG_HPP_071894GER
#define BOOST_UNIT_TEST_LOG_HPP_071894GER

// Boost.Test
#include <boost/test/detail/unit_test_config.hpp>
#include <boost/test/detail/log_level.hpp>
#include <boost/test/fwd_decl.hpp>

// BOOST
#include <boost/utility.hpp>

// STL
#include <iosfwd>   // for std::ostream&
#include <string>   // for std::string&; in fact need only forward declaration

#include <boost/test/detail/suppress_warnings.hpp>

namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************                log manipulators              ************** //
// ************************************************************************** //

struct begin {};

struct end {};

struct line {
    explicit    line( std::size_t ln_ ) : m_line_num( ln_ ) {}

    std::size_t m_line_num;
};

struct file {
    explicit    file( const_string fn_ ) : m_file_name( fn_ ) {}

    const_string m_file_name;
};

struct checkpoint {
    explicit    checkpoint( const_string message_ ) : m_message( message_ ) {}

    const_string m_message;
};

// ************************************************************************** //
// **************             entry_value_collector            ************** //
// ************************************************************************** //

namespace ut_detail {

class entry_value_collector {
public:
    // Constructors
    entry_value_collector() : m_last( true ) {}
    entry_value_collector( entry_value_collector& rhs ) : m_last( true ) { rhs.m_last = false; }
    ~entry_value_collector();

    // collection interface
    entry_value_collector operator<<( const_string );
    entry_value_collector operator<<( checkpoint const& );

private:
    // Data members
    bool    m_last;
};

} // namespace ut_detail

// ************************************************************************** //
// **************                 unit_test_log                ************** //
// ************************************************************************** //

class unit_test_log_t : private boost::noncopyable {
public:
    // instance access method;
    static unit_test_log_t& instance();

    void                start( bool log_build_info = false );
    void                header( counter_t );
    void                finish( counter_t );

    // log configuration methods
    void                set_stream( std::ostream& );
    void                set_threshold_level( log_level );
    void                set_threshold_level_by_name( const_string );
    void                set_format( const_string );
    void                set_formatter( unit_test_log_formatter* );

    // test case scope tracking
    void                test_case_enter( test_case const& );
    void                test_case_exit( test_case const&, long testing_time_in_mks );

    // entry setup
    unit_test_log_t&    operator<<( begin const& );         // begin entry 
    unit_test_log_t&    operator<<( end const& );           // end entry
    unit_test_log_t&    operator<<( file const& );          // set entry file name
    unit_test_log_t&    operator<<( line const& );          // set entry line number
    unit_test_log_t&    operator<<( checkpoint const& );    // set checkpoint
    unit_test_log_t&    operator<<( log_level );            // set entry level
    ut_detail::entry_value_collector operator()( log_level );

    // logging methods
    unit_test_log_t&    operator<<( const_string );
    void                log_progress();
    void                log_exception( log_level, const_string );

private:
    // Constructor
    unit_test_log_t();
}; // unit_test_log_t

namespace {
unit_test_log_t& unit_test_log = unit_test_log_t::instance();
}

// helper macros
#define BOOST_UT_LOG_ENTRY                                                  \
    (boost::unit_test::unit_test_log << boost::unit_test::begin()           \
        << boost::unit_test::file( BOOST_TEST_STRING_LITERAL( __FILE__ ) )  \
        << boost::unit_test::line( __LINE__ ))                              \
/**/

} // namespace unit_test

} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.27  2005/01/30 03:26:29  rogeeff
//  return an ability for explicit end()
//
//  Revision 1.26  2005/01/21 07:30:24  rogeeff
//  to log testing time log formatter interfaces changed
//
//  Revision 1.25  2005/01/18 08:26:12  rogeeff
//  unit_test_log rework:
//     eliminated need for ::instance()
//     eliminated need for << end and ...END macro
//     straitend interface between log and formatters
//     change compiler like formatter name
//     minimized unit_test_log interface and reworked to use explicit calls
//
// ***************************************************************************

#endif // BOOST_UNIT_TEST_LOG_HPP_071894GER

