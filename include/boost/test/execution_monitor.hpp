//  (C) Copyright Gennadiy Rozental 2001-2002.
//  (C) Copyright Beman Dawes 2001.
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
//  Description : defines abstract monitor interfaces and implements execution excepiton
//  The original Boost Test Library included an implementation detail function
//  named catch_exceptions() which caught otherwise uncaught C++ exceptions.
//  It was derived from an existing test framework by Beman Dawes.  The
//  intent was to expand later to catch other detectable but platform dependent
//  error events like Unix signals or Windows structured C exceptions.
//
//  Requests from early adopters of the Boost Test Library included
//  configurable levels of error message detail, elimination of templates,
//  separation of error reporting, and making the catch_exceptions() facilities
//  available as a public interface.  Support for unit testing also stretched
//  the function based design.  Implementation within the header became less
//  attractive due to the need to include many huge system dependent headers,
//  although still preferable in certain cases.
//
//  All those issues have been addressed by introducing the class-based
//  design presented here.
// ***************************************************************************

#ifndef BOOST_EXECUTION_MONITOR_HPP
#define BOOST_EXECUTION_MONITOR_HPP

namespace boost {
    
// ************************************************************************** //
// **************              execution_exception             ************** //
// ************************************************************************** //
    
//  design rationale: fear of being out (or nearly out) of memory.
    
class execution_exception {
public:
    enum error_code {
        //  These values are sometimes used as program return codes.
        //  The particular values have been choosen to avoid conflicts with
        //  commonly used program return codes: values < 100 are often user
        //  assigned, values > 255 are sometimes used to report system errors.
        //  Gaps in values allow for orderly expansion.
        
        no_error               = 0,   // for completeness only; never returned
        user_error             = 200, // user reported non-fatal error
        cpp_exception_error    = 205, // see note (1) below
        system_error           = 210, // see note (2) below
        timeout_error          = 215, // only detectable on certain platforms
        user_fatal_error       = 220, // user reported fatal error
        system_fatal_error     = 225  // see note (2) below
        
        //  Note 1: Only uncaught C++ exceptions are treated as errors.
        //  If the application catches a C++ exception, it will never reach
        //  the execution_monitor.
        
        //  Note 2: These errors include Unix signals and Windows structured
        //  exceptions.  They are often initiated by hardware traps.
        //
        //  The implementation decides what's a fatal_system_exception and what's
        //  just a system_exception.  Fatal errors are so likely to have corrupted
        //  machine state (like a stack overflow or addressing exception) that it
        //  is unreasonable to continue execution.
    };
    
    // Constructor
    execution_exception( error_code ec_, char const* what_msg_ ) // max length 256 inc '\0'
    : m_error_code( ec_ ), m_what( what_msg_ ) {}

    // access methods
    error_code      code() const { return m_error_code; }
    char const*     what() const { return m_what; }

private:
    // Data members
    error_code      m_error_code;
    char const*     m_what;
};

// ************************************************************************** //
// **************               execution_monitor              ************** //
// ************************************************************************** //

class execution_monitor {
public:
    // Destructor
    virtual ~execution_monitor()    {}
    
    int execute( int timeout_ = 0 );  // timeout is in seconds
    //  The timeout argument specifies the seconds that elapse before
    //  a timer_error occurs.  May be ignored on some platforms.
    //
    //  Returns:  Value returned by function().
    //
    //  Effects:  Calls function() inside a try/catch block which also may
    //  include other unspecified platform dependent error detection code.
    //
    //  Throws: execution_exception on an uncaught C++ exception,
    //  a hardware or software signal, trap, or other exception.
    //
    //  Note: execute() doesn't consider it an error for function() to
    //  return a non-zero value.
    
    //  user supplied function called by execute()
    virtual int function() = 0;
    
}; // exception monitor

}  // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.6  2002/09/09 09:07:02  rogeeff
//  descriptions added
//
//  Revision 1.5  2002/08/20 22:24:53  rogeeff
//  all formal arguments trailed with underscore
//
//  Revision 1.4  2002/08/20 08:52:40  rogeeff
//  cvs keywords added
//
//   5 Oct 01  Slightly reformatted to fit Boost.Test V2 (Gennadiy Rozental)
//   4 Jun 01  Initial version (Beman Dawes)

// ***************************************************************************

#endif

