//  (C) Copyright Gennadiy Rozental 2001-2003.
//  Use, modification, and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
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

// ************************************************************************** //
// **************           auto_unit_test_registrar           ************** //
// ************************************************************************** //

namespace boost {
namespace unit_test_framework {
namespace detail {

inline boost::unit_test_framework::test_suite*
auto_unit_test_suite()
{
    static boost::unit_test_framework::test_suite* inst = BOOST_TEST_SUITE( "Auto Unit Test" );

    return inst;
}

struct auto_unit_test_registrar
{
    // Constructor
    explicit auto_unit_test_registrar( test_case* tc ) { auto_unit_test_suite()->add( tc ); }
};

} // detail

} // unit_test_framework

} // namespace boost

// ************************************************************************** //
// **************             BOOST_AUTO_UNIT_TEST             ************** //
// ************************************************************************** //

#define BOOST_AUTO_UNIT_TEST( func_name )                           \
static void func_name();                                            \
static boost::unit_test_framework::detail::auto_unit_test_registrar \
    BOOST_JOIN( test_registrar, __LINE__)                           \
        ( BOOST_TEST_CASE( func_name ) );                           \
static void func_name()                                             \
/**/

// ************************************************************************** //
// **************             BOOST_AUTO_UNIT_TEST             ************** //
// ************************************************************************** //

#ifdef BOOST_AUTO_TEST_MAIN
boost::unit_test_framework::test_suite*
init_unit_test_suite( int /* argc */, char* /* argv */ [] ) {
    return boost::unit_test_framework::detail::auto_unit_test_suite();
}
#endif

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.6  2003/11/06 07:39:36  rogeeff
//  Licence update
//
//  Revision 1.5  2003/11/02 06:16:14  rogeeff
//  fixed to support multi-module unit testing
//
//  Revision 1.4  2003/10/27 07:13:12  rogeeff
//  licence update
//
//  Revision 1.3  2003/06/09 08:40:26  rogeeff
//  1.30.beta1
//

// ***************************************************************************

#endif // BOOST_AUTO_UNIT_TEST_HPP
