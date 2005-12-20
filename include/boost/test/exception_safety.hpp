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
//  Description : Facilities to perform exception safety tests
// ***************************************************************************

#ifndef BOOST_TEST_EXCEPTION_SAFETY_HPP_111705GER
#define BOOST_TEST_EXCEPTION_SAFETY_HPP_111705GER

// Boost.Test
#include <boost/test/detail/config.hpp>
#include <boost/test/utils/callback.hpp>

// STL
#include <memory>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

// ************************************************************************** //
// **************          BOOST_TEST_EXCEPTION_SAFETY         ************** //
// ************************************************************************** //

#define BOOST_TEST_EXCEPTION_SAFETY( test_name )                        \
struct test_name : public BOOST_AUTO_TEST_CASE_FIXTURE                  \
{ void test_method(); };                                                \
                                                                        \
static void BOOST_AUTO_TC_INVOKER( test_name )()                        \
{                                                                       \
    test_name t;                                                        \
    ::boost::itest::exception_safety(                                   \
        boost::bind( &test_name::test_method, t ),                      \
        BOOST_STRINGIZE(test_name) );                                   \
}                                                                       \
                                                                        \
struct BOOST_AUTO_TC_UNIQUE_ID( test_name ) {};                         \
                                                                        \
BOOST_AUTO_TC_REGISTRAR( test_name )(                                   \
    boost::unit_test::make_test_case(                                   \
        &BOOST_AUTO_TC_INVOKER( test_name ), #test_name ),              \
    boost::unit_test::ut_detail::auto_tc_exp_fail<                      \
        BOOST_AUTO_TC_UNIQUE_ID( test_name )>::value );                 \
                                                                        \
void test_name::test_method()                                           \
/**/

namespace boost {

namespace itest {

// ************************************************************************** //
// **************             exception safety test            ************** //
// ************************************************************************** //

void    BOOST_TEST_DECL exception_safety( unit_test::callback0<> const& F, 
                                          unit_test::const_string test_name = "" );

} // namespace itest

} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.2  2005/12/20 23:50:13  rogeeff
//  unit_test.hpp removed
//
//  Revision 1.1  2005/12/14 05:03:46  rogeeff
//  exception safety automatic testing facilties
//
// ***************************************************************************

#endif // BOOST_TEST_EXCEPTION_SAFETY_HPP_111705GER
