//  (C) Copyright Gennadiy Rozental 2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : defines framework singletom object
// ***************************************************************************

#ifndef BOOST_TEST_FRAMEWORK_HPP_020805GER
#define BOOST_TEST_FRAMEWORK_HPP_020805GER

// Boost.Test
#include <boost/test/detail/global_typedef.hpp>
#include <boost/test/detail/fwd_decl.hpp>
#include <boost/test/utils/trivial_singleton.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************                   framework                  ************** //
// ************************************************************************** //

namespace framework {

// initialization
void                init( int argc, char* argv[] );

// mutation access methods
void                register_test_unit( test_case* tc );
void                register_test_unit( test_suite* ts );

void                register_observer( test_observer& );
void                reset_observers();

// constant access methods
test_suite const&   master_test_suite();
test_case const&    current_test_case();
test_unit const&    get( test_unit_id, test_unit_type );
template<typename UnitType>
UnitType const&     get( test_unit_id id )
{
    return static_cast<UnitType const&>( get( id, (test_unit_type)UnitType::type ) );
}

// test initiation
void                run( test_unit_id = INV_TEST_UNIT_ID, bool continue_test = true );
void                run( test_unit const*, bool continue_test = true );

// public test events dispatchers
void                assertion_result( bool passed );
void                exception_caught( execution_exception const& );
void                test_unit_aborted();

} // namespace framework

} // unit_test

} // namespace boost

//____________________________________________________________________________//

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.1  2005/02/20 08:27:05  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
// ***************************************************************************

#endif // BOOST_TEST_FRAMEWORK_HPP_020805GER

