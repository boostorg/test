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

#ifndef BOOST_TEST_AUTO_UNIT_TEST_HPP_071894GER
#define BOOST_TEST_AUTO_UNIT_TEST_HPP_071894GER

// Boost.Test
#include <boost/test/unit_test.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

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
    explicit auto_unit_test_registrar( test_case* tc )
    {
        auto_unit_test_suite()->add( tc );
    }
    explicit auto_unit_test_registrar( test_unit_generator const& tc_gen )
    {
        auto_unit_test_suite()->add( tc_gen );
    }
};

} // namespace ut_detail

} // namespace unit_test

} // namespace boost

// ************************************************************************** //
// **************             BOOST_AUTO_TEST_CASE             ************** //
// ************************************************************************** //

#define BOOST_AUTO_TEST_CASE( func_name )                       \
static void func_name();                                        \
static boost::unit_test::ut_detail::auto_unit_test_registrar    \
BOOST_JOIN( test_registrar, __LINE__)                           \
        ( BOOST_TEST_CASE( func_name ) );                       \
static void func_name()                                         \
/**/

// ************************************************************************** //
// **************        BOOST_AUTO_TEST_CASE_TEMPLATE         ************** //
// ************************************************************************** //

#define BOOST_AUTO_TEST_CASE_TEMPLATE( name, type_name, TL )    \
template<typename type_name>                                    \
void BOOST_JOIN( name, _impl )( boost::type<type_name>* );      \
                                                                \
struct name {                                                   \
    template<typename TestType>                                 \
    static void run( boost::type<TestType>* frwrd = 0 )         \
    {                                                           \
       BOOST_JOIN( name, _impl )( frwrd );                      \
    }                                                           \
};                                                              \
                                                                \
static boost::unit_test::ut_detail::auto_unit_test_registrar    \
BOOST_JOIN( test_registrar, __LINE__)                           \
        ( BOOST_TEST_CASE_TEMPLATE( name, TL ) );               \
                                                                \
template<typename type_name>                                    \
void BOOST_JOIN( name, _impl )( boost::type<type_name>* )       \
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


// deprecated
#define BOOST_AUTO_UNIT_TEST( f ) BOOST_AUTO_TEST_CASE( f )


//____________________________________________________________________________//

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.13  2005/02/20 08:27:05  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
//  Revision 1.12  2005/02/01 06:40:06  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
// ***************************************************************************

#endif // BOOST_TEST_AUTO_UNIT_TEST_HPP_071894GER
