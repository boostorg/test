//  (C) Copyright Gennadiy Rozental 2001-2003.
//  See accompanying license for terms and conditions of use.

//  See http://www.boost.org for most recent version including documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : support for automated test cases registration mechanism
//                for simple function based test cases
// ***************************************************************************

#ifndef BOOST_AUTO_UNIT_TEST_HPP
#define BOOST_AUTO_UNIT_TEST_HPP

// Boost.Test
#include <boost/test/unit_test.hpp>

static boost::unit_test_framework::test_suite* test = BOOST_TEST_SUITE( "Auto Unit Test" );

boost::unit_test_framework::test_suite*
init_unit_test_suite( int /* argc */, char* /* argv */ [] ) {
    return test;
}

// ************************************************************************** //
// **************           auto_unit_test_registrar           ************** //
// ************************************************************************** //

namespace boost {

namespace unit_test_framework {

struct auto_unit_test_registrar
{
    // Constructor
    explicit auto_unit_test_registrar( test_case* tc ) { test->add( tc ); }
};

} // unit_test_framework

} // namespace boost

// ************************************************************************** //
// **************             BOOST_AUTO_UNIT_TEST             ************** //
// ************************************************************************** //

#define BOOST_AUTO_UNIT_TEST( func_name )                           \
static void func_name();                                            \
static boost::unit_test_framework::auto_unit_test_registrar         \
    BOOST_JOIN( test_registrar, __LINE__)                           \
        ( BOOST_TEST_CASE( func_name ) );                           \
static void func_name()                                             \
/**/

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.4  2003/10/27 07:13:12  rogeeff
//  licence update
//
//  Revision 1.3  2003/06/09 08:40:26  rogeeff
//  1.30.beta1
//

// ***************************************************************************

#endif // BOOST_AUTO_UNIT_TEST_HPP
