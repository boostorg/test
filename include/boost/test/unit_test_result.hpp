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
//  Description : defines class unit_test_result that is responsible for 
//  gathering test results and presenting this information to end-user
// ***************************************************************************

#ifndef BOOST_UNIT_TEST_RESULT_HPP_071894GER
#define BOOST_UNIT_TEST_RESULT_HPP_071894GER

// Boost.Test
#include <boost/test/detail/unit_test_config.hpp>
#include <boost/test/fwd_decl.hpp>

// Boost
#include <boost/shared_ptr.hpp>

// STL
#include <iosfwd>                       // std::ostream
#include <cstddef>                      // std::size_t
namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************           first failed assertion hook        ************** //
// ************************************************************************** //

namespace {
inline void first_failed_assertion() {}
}

// ************************************************************************** //
// **************               unit_test_result               ************** //
// ************************************************************************** //

class unit_test_result {
    friend struct unit_test_result_saver;
public:
    // Destructor
    ~unit_test_result();

    // current test results access and management
    static unit_test_result& instance();
    static void     test_case_enter( const_string name_, counter_t expected_failures_ = 0 );
    static void     test_case_exit();
    
    // report format configuration
    static void     set_report_format( const_string reportformat );

    // use to dynamically change amount of errors expected in current test case
    void            increase_expected_failures( counter_t amount = 1 );

    // reporting
    void            report( const_string reportlevel, std::ostream& where_to_ );                    // report by level
    void            confirmation_report( std::ostream& where_to_ );                                 // shortest
    void            short_report( std::ostream& where_to_ )    { report( "short", where_to_ ); }    // short
    void            detailed_report( std::ostream& where_to_ ) { report( "detailed", where_to_ ); } // long

    // test case result
    int             result_code() const;                                                            // to be returned from main
    bool            has_passed() const;                                                             // to manage test cases dependency
    
    // to be used by tool box implementation
    void            inc_failed_assertions();
    void            inc_passed_assertions(); 

    // to be used by monitor to notify that test case thrown exception
    void            caught_exception();

    // used mostly by the Boost.Test unit testing
    void            failures_details( counter_t& num_of_failures_, bool& exception_caught_ );

private:
    // report impl method
    void            report_result( std::ostream& where_to_, std::size_t indent_, bool detailed_ );

    // used to temporarily introduce new results set without polluting current one
    static void     reset_current_result_set();

    // Constructor
    unit_test_result( unit_test_result* parent_, const_string test_case_name_, counter_t expected_failures_ = 0 );
   
    // Data members
    struct Impl;
    boost::shared_ptr<Impl> m_pimpl;
};

// ************************************************************************** //
// **************            unit_test_result_saver            ************** //
// ************************************************************************** //

struct unit_test_result_saver
{
    unit_test_result_saver()  { unit_test_result::reset_current_result_set(); }
    ~unit_test_result_saver() { unit_test_result::reset_current_result_set(); }
};

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.25  2005/02/01 08:59:29  rogeeff
//  supplied_log_formatters split
//  change formatters interface to simplify result interface
//
//  Revision 1.24  2005/02/01 06:40:06  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
//  Revision 1.23  2005/01/30 03:23:06  rogeeff
//  result_tracker class removed
//  counter type renamed
//
// ***************************************************************************

#endif // BOOST_UNIT_TEST_RESULT_HPP_071894GER

