//  (C) Copyright Gennadiy Rozental 2001-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : support for automated test cases registration mechanism
//                for simple function based test cases
// ***************************************************************************

#ifndef BOOST_AUTO_UNIT_TEST_HPP_071894GER
#define BOOST_AUTO_UNIT_TEST_HPP_071894GER

// Boost.Test
#include <boost/test/unit_test.hpp>

// ************************************************************************** //
// **************           auto_unit_test_registrar           ************** //
// ************************************************************************** //

namespace boost {
namespace unit_test {
namespace ut_detail {

inline boost::unit_test::test_suite*
auto_unit_test_suite()
{
    static boost::unit_test::test_suite* inst = BOOST_TEST_SUITE( "Auto Unit Test" );

    return inst;
}

struct auto_unit_test_registrar
{
    // Constructor
    explicit auto_unit_test_registrar( test_case* tc ) { auto_unit_test_suite()->add( tc ); }
};

} // namespace ut_detail

} // namespace unit_test

} // namespace boost

// ************************************************************************** //
// **************             BOOST_AUTO_UNIT_TEST             ************** //
// ************************************************************************** //

#define BOOST_AUTO_UNIT_TEST( func_name )                           \
static void func_name();                                            \
static boost::unit_test::ut_detail::auto_unit_test_registrar        \
    BOOST_JOIN( test_registrar, __LINE__)                           \
        ( BOOST_TEST_CASE( func_name ) );                           \
static void func_name()                                             \
/**/

// ************************************************************************** //
// **************             BOOST_AUTO_TEST_MAIN             ************** //
// ************************************************************************** //

#ifdef BOOST_AUTO_TEST_MAIN
boost::unit_test::test_suite*
init_unit_test_suite( int /* argc */, char* /* argv */ [] ) {
    return boost::unit_test::ut_detail::auto_unit_test_suite();
}
#endif

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.12  2005/02/01 06:40:06  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
// ***************************************************************************

#endif // BOOST_AUTO_UNIT_TEST_HPP_071894GER
