//  (C) Copyright Gennadiy Rozental 2001-2002.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Id$
//
//  Description :
// ***************************************************************************

#ifndef BOOST_UNIT_TEST_RESULT_HPP
#define BOOST_UNIT_TEST_RESULT_HPP

// LOCAL
#include <boost/test/detail/unit_test_config.hpp>
#include <boost/test/detail/grinning_ptr.hpp>

// STL
#include <iosfwd>

namespace boost {

namespace unit_test_framework {

// ************************************************************************** //
// **************               unit_test_result               ************** //
// ************************************************************************** //

class unit_test_result {
public:
    // Destructor
    ~unit_test_result();

    // current test results access and management
    static unit_test_result& instance();
    static void     test_case_start( char const* name, unit_test_counter expected_failures = 0 );
    static void     test_case_end();
    
    // use to dynamically change amount of errors expected in current test case
    void            increase_expected_failures( unit_test_counter amount = 1 );

    // reporting
    void            confirmation_report( std::ostream& where_to );              // shortest
    void            short_report( std::ostream& where_to, int indent = 0 );     // short
    void            detailed_report( std::ostream& where_to, int indent = 0 );  // long
    int             result_code();                                              // to be returned from main

    // to be used by tool box implementation
    void            inc_failed_assertions();
    void            inc_passed_assertions(); 

    // to be used by monitor to notify that test case thrown exception
    void            caught_exception();

    // access method; to be used by unit_test_log
    char const*     test_case_name();

private:
    // Constructor
    unit_test_result( unit_test_result* parent, char const* test_case_name, unit_test_counter expected_failures = 0 );
   
    struct Impl;
    detail::grinning_ptr<Impl> m_pimpl;
};

} // namespace unit_test_framework

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.4  2002/08/20 08:52:40  rogeeff
//  cvs keywords added
//
//   5 Oct 01  Initial version (Gennadiy Rozental)

// ***************************************************************************

#endif // BOOST_UNIT_TEST_RESULT_HPP

