//  (C) Copyright Gennadiy Rozental 2001-2002.
//  (C) Copyright Ullrich Koethe 2001.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.
//
//  File        : $RCSfile$
//
//  Version     : $Id$
//
//  Description : implements specific subclass of Executon Monitor used by Unit
//  Test Framework to monitor test cases run.
// ***************************************************************************

// Boost.Test
#include <boost/test/detail/unit_test_monitor.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/unit_test_result.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>

namespace boost {

namespace unit_test_framework {

namespace detail {

// ************************************************************************** //
// **************               unit_test_monitor              ************** //
// ************************************************************************** //

unit_test_monitor::error_level
unit_test_monitor::execute_and_translate( int timeout ) 
{
    try {
        execute(timeout);
    }
    catch( execution_exception const& exex ) {
        report_level report_level =
             exex.code() <= execution_exception::cpp_exception_error ? report_cpp_exception_errors :
            (exex.code() <= execution_exception::timeout_error       ? report_system_errors :
                                                                       report_fatal_errors);
        unit_test_log::instance() 
            << begin() << level( report_level ) << report_exception( exex.what() ) << end();

        unit_test_result::instance().caught_exception();

        // translate execution_exception::error_code to detail::error_level
        switch( exex.code() ) {
        case execution_exception::no_error:             return test_ok;
        case execution_exception::cpp_exception_error:  return unexpected_exception;
        case execution_exception::system_error:         return os_exception;
        case execution_exception::timeout_error:        return os_timeout;
        case execution_exception::user_fatal_error:
        case execution_exception::system_fatal_error:   return fatal_error;
        default:                                        return unexpected_exception;
        }
    }

    return test_ok;
}

//____________________________________________________________________________//

int
unit_test_monitor::function() 
{
    try {
        (m_test_case.*m_test_case_function)();
    }
    catch( test_toolbox::detail::test_tool_failed const& /*e*/ ) { // e not used; error already reported
        // nothing to do
    }

    return 0;
}

//____________________________________________________________________________//

} // namespace detail

} // namespace unit_test_framework

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.4.2.1  2002/10/01 05:48:27  rogeeff
//  coment clarified
//
//  Revision 1.4  2002/08/20 08:24:13  rogeeff
//  cvs keywords added
//
//   5 Oct 01  Initial version (Gennadiy Rozental)

// ***************************************************************************

// EOF
