//  (C) Copyright Gennadiy Rozental 2001-2003.
//  See accompanying license for terms and conditions of use.

//  See http://www.boost.org for most recent version including documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : defines specific version of execution monitor used to run unit 
//  test cases. Translates executioin exception into error level
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
    typedef void (test_case::*function_to_monitor)();
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

    // Constructor
    unit_test_monitor( test_case& target_test_case_, function_to_monitor f_ )
    : m_test_case( target_test_case_ ), m_test_case_function( f_ ) {}

    // management method; same for all monitors
    static void         catch_system_errors( bool yes_no = true ) { s_catch_system_errors = yes_no; }

    // monitor method
    error_level         execute_and_translate( int timeout_ );

    // execution monitor hook implementation
    virtual int         function();

private:
    // Data members
    test_case&          m_test_case;
    function_to_monitor m_test_case_function;

    static bool         s_catch_system_errors;
}; // unit_test_monitor

} // namespace detail

} // namespace unit_test_framework

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.10  2003/10/27 07:13:13  rogeeff
//  licence update
//
//  Revision 1.9  2003/06/09 08:39:28  rogeeff
//  1.30.beta1
//

// ***************************************************************************

#endif // BOOST_UNIT_TEST_MONITOR_HPP
