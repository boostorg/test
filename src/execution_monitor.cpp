//  (C) Copyright Gennadiy Rozental 2001-2002.
//  (C) Copyright Beman Dawes and Ullrich Koethe 1995-2001.
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
//  Description : provides execution monitor implementation for all supported 
//  configurations, including Microsoft structured exception based, unix signals
//  based and special workarounds for borland
//
//  Note that when testing requirements or user wishes preclude use of this
//  file as a separate compilation uses, it may be #included as a header file.
//
//  Header dependencies are deliberately restricted to reduce coupling to other
//  boost libraries.
// ***************************************************************************

// LOCAL
#include <boost/test/execution_monitor.hpp>

// BOOST
#include <boost/cstdlib.hpp>  // for exit codes
#include <boost/config.hpp>   // for workarounds

// STL
#include <string>             // for string
#include <new>                // for bad_alloc
#include <typeinfo>           // for bad_cast, bad_typeid
#include <exception>          // for exception, bad_exception
#include <stdexcept>          // for std exception hierarchy
#include <cstring>            // for std::strlen etc

#ifdef BOOST_NO_STDC_NAMESPACE
    namespace std { using ::strlen; using ::strncat; }
#endif

// Microsoft + other compatible compilers such as Intel
#if defined(_MSC_VER) || (defined(__MWERKS__) && __MWERKS__ >= 0x3000)

#define BOOST_MS_STRCTURED_EXCEPTION_HANDLING
#include <wtypes.h>
#include <winbase.h>
#include <excpt.h>
#include <eh.h> 

#elif (defined(__BORLANDC__) && defined(_Windows))
#define BOOST_MS_STRCTURED_EXCEPTION_HANDLING
#include <windows.h>  // Borland 5.5.1 has its own way of doing things.

// for testing on Win32, GCC thinks it is a unix platform
// TODO: figure out how to tell it is really unix
#elif defined(__unix) && !defined(__GNUC__)

#define BOOST_UNIX_STYLE_SIGNAL_HANDLING
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

#else
#define BOOST_NO_SIGNAL_HANDLING

#endif

namespace boost {

namespace detail {

//  boost::execution_monitor::execute() calls boost::detail::catch_signals() to
//    execute user function with signals control
//  boost::execution_monitor::execute() calls boost::detail::report_error(...) to
//    report any caught exception and throw execution_exception

static int  catch_signals( execution_monitor & exmon, int timeout ); //  timeout is in seconds. 0 implies none.
static void report_error( execution_exception::error_code ec,
                          char const* msg1, char const* msg2 = "" );

//____________________________________________________________________________//

// Declaration for Microsoft structured exception handling (unix alternative - signal)
#ifdef BOOST_MS_STRCTURED_EXCEPTION_HANDLING

//  this class defined per the Microsoft structured exception documentation
class ms_se_exception {
public:
    // Constructor
    ms_se_exception( unsigned int n ) : m_se_id( n ) {}

    // Destructor
    ~ms_se_exception() {}

    // access methods
    unsigned int id() const { return m_se_id; }

private:
    // Data members
    unsigned int m_se_id;
};

//____________________________________________________________________________//

void ms_se_trans_func( unsigned int id, _EXCEPTION_POINTERS * exps );
static void report_ms_se_error( unsigned int id );

//____________________________________________________________________________//

// Declarations for unix-style signal handling
#elif defined(BOOST_UNIX_STYLE_SIGNAL_HANDLING)

class unix_signal_exception {
public:
    // Constructor
    unix_signal_exception( execution_exception::error_code ec, char const* em )
    : m_error_code( ec ), m_error_message( em )             {}

    // Destructor
    ~unix_signal_exception() {}

    // access methods
    execution_exception::error_code error_code() const      { return m_error_code;    }
    char const*                     error_message() const   { return m_error_message; }
private:
    // Data members
    execution_exception::error_code m_error_code;
    char const*                     m_error_message;
};

#endif

//____________________________________________________________________________//

}  // namespace detail

// ************************************************************************** //
// **************               execution_monitor              ************** //
// ************************************************************************** //

int boost::execution_monitor::execute( int timeout )
{

#if defined(BOOST_MS_STRCTURED_EXCEPTION_HANDLING) && !defined(__BORLANDC__)
    _set_se_translator( detail::ms_se_trans_func );
#endif

    try {
        return detail::catch_signals( *this, timeout );
    }

    //  Catch-clause reference arguments are a bit different from function
    //  arguments (ISO 15.3 paragraphs 18 & 19).  Apparently const isn't
    //  required.  Programmers ask for const anyhow, so we supply it.  That's
    //  easier than answering questions about non-const usage.

    catch( char const* ex )
      { detail::report_error( execution_exception::cpp_exception_error, "C string: ", ex ); }
    catch( std::string const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::string", ex.c_str() ); }

    //  std:: exceptions

    catch( std::bad_alloc const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::bad_alloc: ", ex.what() ); }

#if !defined(__BORLANDC__) || __BORLANDC__ > 0x0551
    catch( std::bad_cast const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::bad_cast: ", ex.what() ); }
    catch( std::bad_typeid const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::bad_typeid: ", ex.what() ); }
#else
    catch( std::bad_cast const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::bad_cast", "" ); }
    catch( std::bad_typeid const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::bad_typeid", "" ); }
#endif

    catch( std::bad_exception const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::bad_exception: ", ex.what() ); }
    catch( std::domain_error const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::domain_error: ", ex.what() ); }
    catch( std::invalid_argument const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::invalid_argument: ", ex.what() ); }
    catch( std::length_error const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::length_error: ", ex.what() ); }
    catch( std::out_of_range const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::out_of_range: ", ex.what() ); }
    catch( std::range_error const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::range_error: ", ex.what() ); }
    catch( std::overflow_error const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::overflow_error: ", ex.what() ); }
    catch( std::underflow_error const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::underflow_error: ", ex.what() ); }
    catch( std::logic_error const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::logic_error: ", ex.what() ); }
    catch( std::runtime_error const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::runtime_error: ", ex.what() ); }
    catch( std::exception const& ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::exception: ", ex.what() ); }

#if   defined(BOOST_MS_STRCTURED_EXCEPTION_HANDLING)
    catch( detail::ms_se_exception const& ex )
      { detail::report_ms_se_error( ex.id() ); }
#elif defined(BOOST_UNIX_STYLE_SIGNAL_HANDLING)
    catch( detail::unix_signal_exception const& ex )
      { detail::report_error( ex.error_code(), ex.error_message() ); }
#endif  // BOOST_UNIX_STYLE_SIGNAL_HANDLING

    catch( execution_exception const& ) { throw; }

    catch( ... )
      { detail::report_error( execution_exception::cpp_exception_error, "unknown type", "" ); }

    return 0;  // never reached; supplied to quiet compiler warnings
} // execute

//____________________________________________________________________________//

namespace detail {

// ************************************************************************** //
// **************          boost::detail::catch_signals        ************** //
// ************************************************************************** //

#if defined(BOOST_UNIX_STYLE_SIGNAL_HANDLING)

inline sigjmp_buf & execution_monitor_jump_buffer()
{
    static sigjmp_buf unit_test_jump_buffer_;
    return unit_test_jump_buffer_;
}

//____________________________________________________________________________//

extern "C" {

static void execution_monitor_signal_handler( int sig )
{
    siglongjmp( execution_monitor_jump_buffer(), sig );
}

}
//____________________________________________________________________________//

int catch_signals( execution_monitor & exmon, int timeout )
{
    static struct sigaction all_signals_action;
    all_signals_action.sa_flags   = 0;
    all_signals_action.sa_handler = &execution_monitor_signal_handler;
    sigemptyset( &all_signals_action.sa_mask );

    struct sigaction old_SIGFPE_action;
    struct sigaction old_SIGTRAP_action;
    struct sigaction old_SIGSEGV_action;
    struct sigaction old_SIGBUS_action;
    struct sigaction old_SIGALRM_action;

    sigaction( SIGFPE , &all_signals_action, &old_SIGFPE_action  );
    sigaction( SIGTRAP, &all_signals_action, &old_SIGTRAP_action );
    sigaction( SIGSEGV, &all_signals_action, &old_SIGSEGV_action );
    sigaction( SIGBUS , &all_signals_action, &old_SIGBUS_action  );

    int                             result;
    execution_exception::error_code ec = execution_exception::no_error;
    char const*                     em;

    if( timeout ) {
        sigaction( SIGALRM , &all_signals_action, &old_SIGALRM_action );
        alarm(timeout);
    }

    volatile int sigtype = sigsetjmp( execution_monitor_jump_buffer(), 1 );
    if( sigtype == 0 ) {
        result = exmon.function();
    }
    else {
        switch(sigtype) {
        case SIGALRM:
            ec = execution_exception::timeout_error;
            em = "signal: SIGALRM (timeout while executing function)";
            break;
        case SIGTRAP:
            ec = execution_exception::system_error;
            em = "signal: SIGTRAP (perhaps integer divide by zero)";
            break;
        case SIGFPE:
            ec = execution_exception::system_error;
            em = "signal: SIGFPE (arithmetic exception)";
            break;
        case SIGSEGV:
        case SIGBUS:
            ec = execution_exception::system_fatal_error;
            em = "signal: memory access violation";
            break;
        default:
            ec = execution_exception::system_error;
            em = "signal: unrecognized signal";
        }
    }
    if( timeout ) {
        alarm(0);
        sigaction( SIGALRM, &old_SIGALRM_action, NULL );
    }
    sigaction( SIGFPE , &old_SIGFPE_action , NULL );
    sigaction( SIGTRAP, &old_SIGTRAP_action, NULL );
    sigaction( SIGSEGV, &old_SIGSEGV_action, NULL );
    sigaction( SIGBUS , &old_SIGBUS_action , NULL );

    if( ec != execution_exception::no_error ) {
        throw unix_signal_exception( ec, em );
    }

    return result;
}  // unix catch_signals

//____________________________________________________________________________//

#elif (defined(__BORLANDC__) && defined(_Windows))

// this works for Borland but not other Win32 compilers (which trap too many cases)
int catch_signals( execution_monitor & exmon, int )
{
    int result;

    __try { result = exmon.function(); }

    __except (1)
    {
        throw ms_se_exception( GetExceptionCode() );
    }

    return result;
}

#else  // default signal handler

int catch_signals( execution_monitor & exmon, int )
{
    return exmon.function();
}

#endif  // choose signal handler

// ************************************************************************** //
// **************   Microsoft structured exception handling    ************** //
// ************************************************************************** //

#if defined(BOOST_MS_STRCTURED_EXCEPTION_HANDLING)

void
ms_se_trans_func( unsigned int id, _EXCEPTION_POINTERS* /* exps */ )
{
    throw ms_se_exception( id );
}

//____________________________________________________________________________//

void
report_ms_se_error( unsigned int id )
{
    switch( id ) {
        // cases classified as fatal_system_error
    case EXCEPTION_ACCESS_VIOLATION:
        detail::report_error( execution_exception::system_fatal_error,
            "memory access violation", "" );
        break;

    case EXCEPTION_ILLEGAL_INSTRUCTION:
        detail::report_error( execution_exception::system_fatal_error,
            "illegal instruction", "" );
        break;

    case EXCEPTION_PRIV_INSTRUCTION:
        detail::report_error( execution_exception::system_fatal_error,
            "privilaged instruction", "" );
        break;

    case EXCEPTION_IN_PAGE_ERROR:
        detail::report_error( execution_exception::system_fatal_error,
            "memory page error", "" );
        break;

    case EXCEPTION_STACK_OVERFLOW:
        detail::report_error( execution_exception::system_fatal_error,
            "stack overflow", "" );
        break;

        // cases classified as (non-fatal) system_trap
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        detail::report_error( execution_exception::system_error,
            "data misalignment", "" );
        break;

    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        detail::report_error( execution_exception::system_error,
            "integer divide by zero", "" );
        break;

    case EXCEPTION_INT_OVERFLOW:
        detail::report_error( execution_exception::system_error,
            "integer overflow", "" );
        break;

    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        detail::report_error( execution_exception::system_error,
            "array bounds exceeded", "" );
        break;

    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        detail::report_error( execution_exception::system_error,
            "floating point divide by zero", "" );
        break;

    case EXCEPTION_FLT_STACK_CHECK:
        detail::report_error( execution_exception::system_error,
            "floating point stack check", "" );
        break;

    case EXCEPTION_FLT_DENORMAL_OPERAND:
    case EXCEPTION_FLT_INEXACT_RESULT:
    case EXCEPTION_FLT_INVALID_OPERATION:
    case EXCEPTION_FLT_OVERFLOW:
    case EXCEPTION_FLT_UNDERFLOW:
        detail::report_error( execution_exception::system_error,
            "floating point error", "" );
        break;

    default:
        detail::report_error( execution_exception::system_error,
            "unrecognized exception or signal", "" );
    }  // switch
}  // report_ms_se_error

//____________________________________________________________________________//

#endif  // Microsoft structured exception handling

// ************************************************************************** //
// **************                  report_error                ************** //
// ************************************************************************** //

static void report_error( execution_exception::error_code ec, char const* msg1, char const* msg2 )
{
    static char buf[512];
    buf[0] = '\0';
    std::strncat( buf, msg1, sizeof(buf)-1 );
    std::strncat( buf, msg2, sizeof(buf)-1-std::strlen(buf) );
    throw execution_exception( ec, buf );
}

//____________________________________________________________________________//

} // namespace detail

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.10  2002/09/15 11:17:23  johnmaddock
//  Added missing includes
//
//  Revision 1.9  2002/09/04 07:27:08  rogeeff
//  space before colon deleted
//
//  Revision 1.8  2002/08/22 11:50:39  johnmaddock
//  Added Kylix 3 support
//
//  Revision 1.7  2002/08/20 08:24:13  rogeeff
//  cvs keywords added
//
//   5 Oct 01 Slightly reworked:  (Gennadiy Rozental)
//   5 Jun 01 Made SE code work with several Win32 compilers (Beman)
//   4 Jun 01 New interface to old code (was catch_exceptions.hpp)
//            to avoid use of template as requested by users (Beman)
//   4 Apr 01 Added signal handling code. (Ullrich)
//   4 Apr 01 Removed default output at end of exception handling (Ullrich)
//  26 Feb 01 Numerous changes suggested during formal review (Beman)
//  25 Jan 01 catch_exceptions.hpp code factored out of cpp_main.cpp
//  22 Jan 01 Remove test_tools dependencies to reduce coupling
//   5 Nov 00 Initial boost version (Beman Dawes)

// ***************************************************************************

// EOF

