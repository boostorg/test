//  (C) Copyright Gennadiy Rozental & Ullrich Koethe 2001.
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

#ifndef BOOST_UNIT_TEST_MONITOR_HPP
#define BOOST_UNIT_TEST_MONITOR_HPP

#include <boost/test/execution_monitor.hpp>

namespace boost {

namespace unit_test_framework {

class test_case;

namespace detail {

// ************************************************************************** //
// **************               unit_test_monitor              ************** //
// ************************************************************************** //

class unit_test_monitor : public execution_monitor {
public:
    enum error_level { 
        test_fail               =  1,
        test_ok                 =  0,
        constructor_error       = -1, 
        unexpected_exception    = -2, 
        os_exception            = -3, 
        os_timeout              = -4, 
        fatal_error             = -5,  // includes both system and user
        destructor_error        = -6
    };
    static bool         is_critical_error( error_level e_ ) { return e_ <= fatal_error; }

    typedef void (test_case::*function_to_monitor)();

    // Constructor
    unit_test_monitor( test_case& target_test_case_, function_to_monitor f_ )
    : m_test_case_function( f_ ), m_test_case( target_test_case_ ) {}

    // monitor method
    error_level         execute_and_translate( int timeout_ );

    // execution monitor hook implementation
    virtual int         function();

private:
    // Data members
    function_to_monitor m_test_case_function;
    test_case&          m_test_case;
}; // unit_test_monitor

} // namespace detail

} // namespace unit_test_framework

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.5  2002/09/04 07:28:11  rogeeff
//  comment typo
//
//  Revision 1.4  2002/08/20 22:24:53  rogeeff
//  all formal arguments trailed with underscore
//
//  Revision 1.3  2002/08/20 08:52:41  rogeeff
//  cvs keywords added
//
//   5 Oct 01  Initial version (Gennadiy Rozental)

// ***************************************************************************

#endif // BOOST_UNIT_TEST_MONITOR_HPP
