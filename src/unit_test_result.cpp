//  (C) Copyright Gennadiy Rozental 2001-2002.
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
//  Description : implements Unit Test Result reporting facility. Note that majority of 
//  implementation is hidden in this file using pimple idiom.
// ***************************************************************************

// Boost.Test
#include <boost/test/unit_test_result.hpp>
#include <boost/test/unit_test_suite.hpp>

// BOOST
#include <boost/config.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/cstdlib.hpp>

// STL
#include <list>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <cmath>

# ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::log10; using ::strncmp; }
# endif

namespace boost {

namespace unit_test_framework {

typedef unit_test_result* unit_test_result_ptr;

// ************************************************************************** //
// **************               unit_test_result               ************** //
// ************************************************************************** //

struct unit_test_result::Impl {
    unit_test_result_ptr            m_parent;
    std::list<unit_test_result_ptr> m_children;
    unit_test_counter               m_assertions_passed;
    unit_test_counter               m_assertions_failed;
    unit_test_counter               m_expected_failures;
    unit_test_counter               m_test_cases_passed;
    unit_test_counter               m_test_cases_failed;
    bool                            m_exception_caught;
    c_string_literal                m_test_case_name;

    static boost::scoped_ptr<unit_test_result> m_head;
    static unit_test_result_ptr     m_curr;

    bool                            is_failed()   { return m_assertions_failed != m_expected_failures || m_exception_caught; }
    int                             result_code()
    { 
        return is_failed() 
                    ? ( (m_assertions_failed != 0) 
                            ? boost::exit_test_failure 
                            : boost::exit_exception_failure )
                    : boost::exit_success; 
    }
};

boost::scoped_ptr<unit_test_result> unit_test_result::Impl::m_head;
unit_test_result_ptr unit_test_result::Impl::m_curr = unit_test_result_ptr();

//____________________________________________________________________________//

unit_test_result::unit_test_result( unit_test_result_ptr parent, c_string_literal test_case_name, unit_test_counter exp_fail )
: m_pimpl( new Impl )
{
    m_pimpl->m_parent            = parent;
    m_pimpl->m_test_case_name    = test_case_name;

    m_pimpl->m_assertions_passed = 0;
    m_pimpl->m_assertions_failed = 0;
    m_pimpl->m_expected_failures = exp_fail;
    m_pimpl->m_test_cases_passed = 0;
    m_pimpl->m_test_cases_failed = 0;
    m_pimpl->m_exception_caught  = false;

}

//____________________________________________________________________________//

static void safe_delete( unit_test_result_ptr ptr ) { boost::checked_delete<unit_test_result>( ptr ); }

unit_test_result::~unit_test_result()
{
    std::for_each( m_pimpl->m_children.begin(), m_pimpl->m_children.end(), &safe_delete );
}

//____________________________________________________________________________//

unit_test_result&
unit_test_result::instance()
{
    assert( Impl::m_head );

    return Impl::m_curr ? *Impl::m_curr : *Impl::m_head;
}

//____________________________________________________________________________//

void
unit_test_result::test_case_start( c_string_literal name, unit_test_counter expected_failures )
{
    unit_test_result_ptr new_result_set = new unit_test_result( Impl::m_curr, name, expected_failures );

    if( Impl::m_curr )
        Impl::m_curr->m_pimpl->m_children.push_back( new_result_set );
    else
        Impl::m_head.reset( new_result_set );

    Impl::m_curr = new_result_set;
}

//____________________________________________________________________________//

void
unit_test_result::test_case_end()
{
    assert( Impl::m_curr );

    Impl*                curr_impl  = Impl::m_curr->m_pimpl.get();
    unit_test_result_ptr parent     = curr_impl->m_parent;

    if( parent ) {
        // accumulate results
        parent->m_pimpl->m_assertions_passed += curr_impl->m_assertions_passed;
        parent->m_pimpl->m_assertions_failed += curr_impl->m_assertions_failed;
        parent->m_pimpl->m_test_cases_passed += curr_impl->m_test_cases_passed;
        parent->m_pimpl->m_test_cases_failed += curr_impl->m_test_cases_failed;

        // for test_cases (vs. test_suite)
        if( curr_impl->m_test_cases_passed == 0 && curr_impl->m_test_cases_failed == 0 ) {
            if( curr_impl->is_failed() )
                parent->m_pimpl->m_test_cases_failed++;
            else
                parent->m_pimpl->m_test_cases_passed++;
        }

    }

    Impl::m_curr = parent;
}

//____________________________________________________________________________//

void
unit_test_result::increase_expected_failures( unit_test_counter amount ) 
{
    m_pimpl->m_expected_failures += amount;

    if( m_pimpl->m_parent )
        m_pimpl->m_parent->increase_expected_failures( amount );
}

//____________________________________________________________________________//

void
unit_test_result::inc_failed_assertions()
{
    m_pimpl->m_assertions_failed++;
}

//____________________________________________________________________________//

void
unit_test_result::inc_passed_assertions()
{
    m_pimpl->m_assertions_passed++;
}

//____________________________________________________________________________//

void
unit_test_result::caught_exception()
{
    m_pimpl->m_exception_caught = true;
}

//____________________________________________________________________________//

c_string_literal
unit_test_result::test_case_name()
{
    return m_pimpl->m_test_case_name;
}

//____________________________________________________________________________//

void
unit_test_result::reset_current_result_set()
{
    static unit_test_result_ptr backup = unit_test_result_ptr();
    static boost::scoped_ptr<unit_test_result> temporary_substitute;

    assert( Impl::m_curr );

    if( backup ) {
        Impl::m_curr = backup;
        backup = unit_test_result_ptr();
        temporary_substitute.reset();
    }
    else {
        backup = Impl::m_curr;
        Impl::m_curr = new unit_test_result( NULL, Impl::m_curr->test_case_name(), 0 );
        temporary_substitute.reset( Impl::m_curr );
    }
}

//____________________________________________________________________________//

void
unit_test_result::failures_details( unit_test_counter& num_of_failures, bool& exception_caught )
{
    num_of_failures  = m_pimpl->m_assertions_failed;
    exception_caught = m_pimpl->m_exception_caught;
}

//____________________________________________________________________________//

namespace {
std::string ps_name( bool p_s, c_string_literal singular_form )  { return p_s ? std::string( singular_form ).append( "s" ) : std::string( singular_form ); }
std::string cs_name( bool c_s )                                  { return c_s ? "case" : "suite"; }
std::string quote( c_string_literal name )                       { return std::string( " \"" ).append( name ).append( "\" "); }
}

//____________________________________________________________________________//

void
unit_test_result::confirmation_report( std::ostream& where_to )
{
    assert( this );

    bool failed     = m_pimpl->m_test_cases_failed != 0 || m_pimpl->is_failed();

    if( failed ) {
        if( m_pimpl->m_assertions_failed > 0 ) {
            where_to << "\n*** " << m_pimpl->m_assertions_failed 
                     << ps_name( m_pimpl->m_assertions_failed != 1, " failure" );

            if( m_pimpl->m_expected_failures != 0 ) {
            where_to << " (" << m_pimpl->m_expected_failures
                     << ps_name( m_pimpl->m_expected_failures != 1, " failure" ) << " expected) ";
            }

            where_to << " in test " << cs_name( m_pimpl->m_children.empty() ) 
                     << quote( m_pimpl->m_test_case_name );
        }
        if( m_pimpl->m_exception_caught )
            where_to << "\n*** Test " << cs_name( m_pimpl->m_children.empty() )
                     << quote( m_pimpl->m_test_case_name ) << " was aborted due to uncaught exception, user assert or system error";
        where_to << std::endl;
    }
    else
        where_to << "\n*** No errors detected" << std::endl;
}

//____________________________________________________________________________//

void
unit_test_result::short_report( std::ostream& where_to, size_t indent )
{
    assert( this );

    bool              failed           = m_pimpl->m_test_cases_failed != 0 || m_pimpl->is_failed();
    unit_test_counter total_test_cases = m_pimpl->m_test_cases_failed + m_pimpl->m_test_cases_passed;
    unit_test_counter total_assertions = m_pimpl->m_assertions_failed + m_pimpl->m_assertions_passed;
    
    std::string indent_string( indent*2, ' ' );

    where_to << "\n" << indent_string << "Test " << cs_name( m_pimpl->m_children.empty() )
             << quote( m_pimpl->m_test_case_name )
             << (failed ? "failed with:\n" : "passed with:\n");

    if( total_test_cases > 1 ) {
        int width = static_cast<int>( std::log10( (float)std::max( m_pimpl->m_test_cases_passed, 
                                                                   m_pimpl->m_test_cases_failed ) ) ) + 1;
        where_to << indent_string << std::setw( width ) << m_pimpl->m_test_cases_passed
                 << " test " << ps_name( m_pimpl->m_test_cases_passed != 1, "case" ) << " out of " << total_test_cases << " passed\n";
        where_to << indent_string << std::setw( width ) << m_pimpl->m_test_cases_failed
                 << " test " << ps_name( m_pimpl->m_test_cases_failed != 1, "case" ) << " out of " << total_test_cases << " failed\n";
    }

    int width = total_assertions > 0 
                  ? static_cast<int>( std::log10( (float)std::max( m_pimpl->m_assertions_passed, 
                                                                   m_pimpl->m_assertions_failed ) ) ) + 1
                  : 1;

    where_to << indent_string << std::setw( width ) << m_pimpl->m_assertions_passed 
             << ps_name( m_pimpl->m_assertions_passed != 1, " assertion" ) << " out of "
             << total_assertions << " passed\n";
    where_to << indent_string << std::setw( width ) << m_pimpl->m_assertions_failed 
             << ps_name( m_pimpl->m_assertions_failed != 1, " assertion" ) << " out of "
             << total_assertions << " failed\n";

    if( m_pimpl->m_expected_failures > 0 ) {
        where_to << indent_string << "while " << m_pimpl->m_expected_failures 
                 << ps_name( m_pimpl->m_expected_failures != 1, " failure" ) << " expected\n";
    }

    if( m_pimpl->m_exception_caught )
        where_to << indent_string << "Test " << cs_name( m_pimpl->m_children.empty() )
                 << " was aborted due to uncaught exception, user assert or system error" << std::endl;

    where_to.flush();
}

//____________________________________________________________________________//

void
unit_test_result::detailed_report( std::ostream& where_to, size_t indent )
{
    short_report( where_to, indent );

    std::list<unit_test_result_ptr>::iterator it  = m_pimpl->m_children.begin();
    std::list<unit_test_result_ptr>::iterator end = m_pimpl->m_children.end();

    while( it != end ) {
        unit_test_result_ptr next = *(it++);
        next->detailed_report( where_to, indent+1 );
    }
}

//____________________________________________________________________________//

int
unit_test_result::result_code()
{
    return m_pimpl->result_code();
}

//____________________________________________________________________________//

} // namespace unit_test_framework

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.12  2002/12/08 18:16:57  rogeeff
//  eliminated all uses of NULL
//  switched to use c_string_literal
//
//  Revision 1.11  2002/11/02 20:04:42  rogeeff
//  release 1.29.0 merged into the main trank
//

// ***************************************************************************

// EOF
