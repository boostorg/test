//  (C) Copyright Gennadiy Rozental 2001-2005.
//  (C) Copyright Ullrich Koethe 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : implements specific subclass of Executon Monitor used by Unit
//  Test Framework to monitor test cases run.
// ***************************************************************************

#ifndef BOOST_UNIT_TEST_MONITOR_IPP_012205GER
#define BOOST_UNIT_TEST_MONITOR_IPP_012205GER

// Boost.Test
#include <boost/test/detail/unit_test_monitor.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/unit_test_result.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>

namespace boost {

namespace unit_test {

namespace ut_detail {

// ************************************************************************** //
// **************               unit_test_monitor              ************** //
// ************************************************************************** //

bool unit_test_monitor::s_catch_system_errors = true;

unit_test_monitor::error_level
unit_test_monitor::execute_and_translate( test_case* target_test_case, function_to_monitor f, int timeout )
{
    m_test_case         = target_test_case;
    m_test_case_method  = f;

    try {
        execute( s_catch_system_errors, timeout );
    }
    catch( execution_exception const& exex ) {
        log_level loglevel =
             exex.code() <= execution_exception::cpp_exception_error ? log_cpp_exception_errors :
            (exex.code() <= execution_exception::timeout_error       ? log_system_errors :
                                                                       log_fatal_errors);
        unit_test_log.log_exception( loglevel, exex.what() );

        unit_test_result::instance().caught_exception();

        // translate execution_exception::error_code to error_level
        switch( exex.code() ) {
        case execution_exception::no_error:             return test_ok;
        case execution_exception::user_error:           return unexpected_exception;
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
        (m_test_case->*m_test_case_method)();
    }
    catch( test_tools::tt_detail::test_tool_failed const& /*e*/ ) { // e not used; error already reported
        // nothing to do
    }

    return 0;
}

//____________________________________________________________________________//

} // namespace ut_detail

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.2  2005/02/01 06:40:07  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
//  Revision 1.1  2005/01/22 19:22:13  rogeeff
//  implementation moved into headers section to eliminate dependency of included/minimal component on src directory
//
//  Revision 1.17  2005/01/19 16:34:07  vawjr
//  Changed the \r\r\n back to \r\n on windows so we don't get errors when compiling
//  on VC++8.0.  I don't know why Microsoft thinks it's a good idea to call this an error,
//  but they do.  I also don't know why people insist on checking out files on Windows and
//  copying them to a unix system to check them in (which will cause exactly this problem)
//
//  Revision 1.16  2005/01/18 08:30:08  rogeeff
//  unit_test_log rework:
//     eliminated need for ::instance()
//     eliminated need for << end and ...END macro
//     straitend interface between log and formatters
//     change compiler like formatter name
//     minimized unit_test_log interface and reworked to use explicit calls
//
// ***************************************************************************

#endif // BOOST_UNIT_TEST_MONITOR_IPP_012205GER
