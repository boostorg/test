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

// LOCAL
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

// ************************************************************************** //
// **************               unit_test_result               ************** //
// ************************************************************************** //

struct unit_test_result::Impl {
    unit_test_result*               m_parent;
    std::list<unit_test_result*>    m_children;
    unit_test_counter               m_assertions_passed;
    unit_test_counter               m_assertions_failed;
    unit_test_counter               m_expected_failures;
    unit_test_counter               m_test_cases_passed;
    unit_test_counter               m_test_cases_failed;
    bool                            m_exception_caught;
    char const*                     m_test_case_name;

    static boost::scoped_ptr<unit_test_result> m_head;
    static unit_test_result*        m_curr;

    bool                            is_failed()   { return m_assertions_failed != m_expected_failures || m_exception_caught; }
    int                             result_code() { return is_failed() 
                                                        ? ( (m_assertions_failed != 0) 
                                                               ? boost::exit_test_failure 
                                                               : boost::exit_exception_failure  )
                                                        : boost::exit_success; }
};

boost::scoped_ptr<unit_test_result> unit_test_result::Impl::m_head;
unit_test_result* unit_test_result::Impl::m_curr = NULL;

//____________________________________________________________________________//

unit_test_result::unit_test_result( unit_test_result* parent, char const* test_case_name, unit_test_counter exp_fail )
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

static void safe_delete( unit_test_result* ptr ) { boost::checked_delete<unit_test_result>( ptr ); }

unit_test_result::~unit_test_result()
{
    std::for_each( m_pimpl->m_children.begin(), m_pimpl->m_children.end(), &safe_delete );
}

//____________________________________________________________________________//

unit_test_result&
unit_test_result::instance()
{
    assert( Impl::m_head.get() != NULL );

    return Impl::m_curr == NULL ? *Impl::m_head : *Impl::m_curr;
}

//____________________________________________________________________________//

void
unit_test_result::test_case_start( char const* name, unit_test_counter expected_failures )
{
    unit_test_result* new_result_set = new unit_test_result( Impl::m_curr, name, expected_failures );

    if( Impl::m_curr == NULL ) {
        Impl::m_head.reset( new_result_set );
    }
    else
        Impl::m_curr->m_pimpl->m_children.push_back( new_result_set );

    Impl::m_curr = new_result_set;
}

//____________________________________________________________________________//

void
unit_test_result::test_case_end()
{
    assert( Impl::m_curr != NULL );

    Impl*             curr_impl = Impl::m_curr->m_pimpl.get();
    unit_test_result* parent    = curr_impl->m_parent;

    if( parent != NULL ) {
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

    if( m_pimpl->m_parent != NULL )
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

char const*
unit_test_result::test_case_name()
{
    return m_pimpl->m_test_case_name;
}

//____________________________________________________________________________//

namespace {
std::string ps_name( bool p_s, char const* singular_form )  { return p_s ? std::string( singular_form ).append( "s" ) : std::string( singular_form ); }
std::string cs_name( bool c_s )                             { return c_s ? "case" : "suite"; }
std::string quote( char const* name )                       { return std::string( " \"" ).append( name ).append( "\" "); }
}

//____________________________________________________________________________//

void
unit_test_result::confirmation_report( std::ostream& where_to )
{
    assert( this != NULL );

    bool failed     = m_pimpl->m_test_cases_failed != 0 || m_pimpl->is_failed();
    bool case_suite = m_pimpl->m_test_cases_failed + m_pimpl->m_test_cases_passed <= 1;

    if( failed ) {
        if( m_pimpl->m_assertions_failed > 0 ) {
            where_to << "\n*** " << m_pimpl->m_assertions_failed 
                     << ps_name( m_pimpl->m_assertions_failed != 1, " failure" );

            if( m_pimpl->m_expected_failures != 0 ) {
            where_to << " (" << m_pimpl->m_expected_failures
                     << ps_name( m_pimpl->m_expected_failures != 1, " failure" ) << " expected) ";
            }

            where_to << " in test " << cs_name( case_suite ) 
                     << quote( m_pimpl->m_test_case_name );
        }
        if( m_pimpl->m_exception_caught )
            where_to << "\n*** Test " << cs_name( case_suite )
                     << quote( m_pimpl->m_test_case_name ) << "is aborted due to uncaught exception or fatal error";
        where_to << std::endl;
    }
    else
        where_to << "\n*** No errors detected" << std::endl;
}

//____________________________________________________________________________//

void
unit_test_result::short_report( std::ostream& where_to, int indent )
{
    assert( this != NULL );

    bool              failed           = m_pimpl->m_test_cases_failed != 0 || m_pimpl->is_failed();
    unit_test_counter total_test_cases = m_pimpl->m_test_cases_failed + m_pimpl->m_test_cases_passed;
    unit_test_counter total_assertions = m_pimpl->m_assertions_failed + m_pimpl->m_assertions_passed;
    
    std::string indent_string( indent*2, ' ' );

    where_to << "\n" << indent_string << "Test " << cs_name( total_test_cases <= 1 )
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

    where_to.flush();
}

//____________________________________________________________________________//

void
unit_test_result::detailed_report( std::ostream& where_to, int indent )
{
    short_report( where_to, indent );

    std::list<unit_test_result*>::iterator it  = m_pimpl->m_children.begin();
    std::list<unit_test_result*>::iterator end = m_pimpl->m_children.end();

    while( it != end ) {
        unit_test_result* next = *(it++);
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
//  Revision 1.9  2002/08/21 15:15:12  rogeeff
//  previos fix finished
//
//  Revision 1.8  2002/08/20 22:10:31  rogeeff
//  slightly modified failures report
//
//  Revision 1.7  2002/08/20 08:24:13  rogeeff
//  cvs keywords added
//
//   5 Oct 01  Initial version (Gennadiy Rozental)

// ***************************************************************************

// EOF
