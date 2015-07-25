//  (C) Copyright Gennadiy Rozental 2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : unit test for comamnd line parameter parsing framework
// ***************************************************************************

// Boost.Test
#define BOOST_TEST_MODULE Boost.Test CLA parser test
#include <boost/test/unit_test.hpp>
#include <boost/test/utils/runtime/cla/argv_traverser.hpp>
#include <boost/test/utils/runtime/parameter.hpp>

namespace rt = boost::runtime;

BOOST_AUTO_TEST_SUITE( test_argv_traverser )

BOOST_AUTO_TEST_CASE( test_construction )
{
    char const* argv1[] = { "test.exe" };
    rt::cla::argv_traverser tr1( sizeof(argv1)/sizeof(char const*), argv1 );

    BOOST_TEST( tr1.eoi() );

    char const* argv2[] = { "test.exe", "--abc=1" };
    rt::cla::argv_traverser tr2( sizeof(argv2)/sizeof(char const*), argv2 );

    BOOST_TEST( !tr2.eoi() );

    char const* argv3[] = { "test.exe", "" };
    rt::cla::argv_traverser tr3( sizeof(argv3)/sizeof(char const*), argv3 );

    BOOST_TEST( !tr3.eoi() );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_get_char )
{
    char const* argv1[] = { "test.exe", "ab c", "de", "" };
    rt::cla::argv_traverser tr1( sizeof(argv1)/sizeof(char const*), argv1 );

    BOOST_TEST( tr1.get_char() == 'a' );
    BOOST_TEST( tr1.get_char() == 'b' );
    BOOST_TEST( tr1.get_char() == ' ' );
    BOOST_TEST( tr1.get_char() == 'c' );
    BOOST_TEST( tr1.get_char() == rt::cla::END_OF_TOKEN );
    BOOST_TEST( tr1.get_char() == 'd' );
    BOOST_TEST( tr1.get_char() == 'e' );
    BOOST_TEST( !tr1.eoi() );
    BOOST_TEST( tr1.get_char() == rt::cla::END_OF_TOKEN );
    BOOST_TEST( !tr1.eoi() );
    BOOST_TEST( tr1.get_char() == rt::cla::END_OF_TOKEN );
    BOOST_TEST( tr1.eoi() );
    BOOST_TEST( tr1.get_char() == rt::cla::END_OF_TOKEN );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_get_token )
{
    char const* argv1[] = { "test.exe", "abc", "zxcvb", "as kl", "--ooo=111", "a", "" };
    rt::cla::argv_traverser tr1( sizeof(argv1)/sizeof(char const*), argv1 );

    BOOST_TEST( tr1.get_token() == "abc" );
    BOOST_TEST( tr1.get_char() == 'z' );
    BOOST_TEST( tr1.get_token() == "xcvb" );
    BOOST_TEST( tr1.get_token() == "as kl" );
    BOOST_TEST( tr1.get_char() == '-' );
    BOOST_TEST( tr1.get_token() == "-ooo=111" );
    BOOST_TEST( tr1.get_char() == 'a' );
    BOOST_TEST( !tr1.eoi() );
    BOOST_TEST( tr1.get_token() == rt::cstring() );
    BOOST_TEST( !tr1.eoi() );
    BOOST_TEST( tr1.get_token() == rt::cstring() );
    BOOST_TEST( tr1.eoi() );
    BOOST_TEST( tr1.get_token() == rt::cstring() );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_match )
{
    char const* argv1[] = { "test.exe", "abcjkl", "zxcvb", "a" };
    rt::cla::argv_traverser tr1( sizeof(argv1)/sizeof(char const*), argv1 );

    BOOST_TEST( tr1.match( "" ) );
    BOOST_TEST( !tr1.match( "qwe" ) );
    BOOST_TEST( tr1.match( "abc" ) );
    BOOST_TEST( tr1.match( "jkl" ) );
    BOOST_TEST( tr1.match( "" ) );
    BOOST_TEST( !tr1.match( "z" ) );
    BOOST_TEST( tr1.get_token() == rt::cstring() );
    BOOST_TEST( tr1.get_char() == 'z' );
    BOOST_TEST( !tr1.match( "xcva" ) );
    BOOST_TEST( tr1.match( "xcvb" ) );
    BOOST_TEST( tr1.get_char() == rt::cla::END_OF_TOKEN );

    BOOST_TEST( !tr1.match( "c" ) );
    BOOST_TEST( tr1.match( "a" ) );
    BOOST_TEST( !tr1.eoi() );
    BOOST_TEST( tr1.get_char() == rt::cla::END_OF_TOKEN );
    BOOST_TEST( tr1.eoi() );
    BOOST_TEST( !tr1.match( "a" ) );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_remainder )
{
    char const* argv[] = { "test.exe", "abcjkl", "zx vb" };
    rt::cla::argv_traverser tr( sizeof(argv)/sizeof(char const*), argv );

    int new_argc = tr.remainder();

    BOOST_TEST( new_argc == 3 );
    BOOST_TEST( argv[0] == "test.exe" );
    BOOST_TEST( argv[1] == "abcjkl" );
    BOOST_TEST( argv[2] == "zx vb" );

    tr.get_char();
    new_argc = tr.remainder();

    BOOST_TEST( new_argc == 3 );
    BOOST_TEST( argv[0] == "test.exe" );
    BOOST_TEST( argv[1] == "bcjkl" );
    BOOST_TEST( argv[2] == "zx vb" );

    tr.get_token();
    new_argc = tr.remainder();

    BOOST_TEST( new_argc == 2 );
    BOOST_TEST( argv[0] == "test.exe" );
    BOOST_TEST( argv[1] == "zx vb" );

    tr.match( "zx" );
    new_argc = tr.remainder();

    BOOST_TEST( new_argc == 2 );
    BOOST_TEST( argv[0] == "test.exe" );
    BOOST_TEST( argv[1] == " vb" );

    tr.match( " vb" );
    new_argc = tr.remainder();

    BOOST_TEST( new_argc == 2 );
    BOOST_TEST( argv[0] == "test.exe" );
    BOOST_TEST( argv[1] == "" );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE_END()

//____________________________________________________________________________//
//____________________________________________________________________________//
//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE( test_parameter_specification )

BOOST_AUTO_TEST_CASE( test_param_construction )
{
    rt::parameter<int> p1( "P1" );

    BOOST_TEST( p1.p_name.get() == "P1" );
    BOOST_TEST( p1.p_description.get() == "" );
    BOOST_TEST( p1.p_env_var.get() == "" );
    BOOST_TEST( p1.p_optional );
    BOOST_TEST( !p1.p_multiplicable );
    BOOST_TEST( !p1.p_optional_value );

    rt::parameter<int> p2( "P2", (
        rt::required,
        rt::description = "123",
        rt::env_var = "E2",
        rt::multiplicable,
        !rt::optional_value
    ));

    BOOST_TEST( p2.p_name.get() == "P2" );
    BOOST_TEST( p2.p_description.get() == "123" );
    BOOST_TEST( p2.p_env_var.get() == "E2" );
    BOOST_TEST( !p2.p_optional );
    BOOST_TEST( p2.p_multiplicable );
    BOOST_TEST( !p1.p_optional_value );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_param_store )
{
    rt::parameter<int> p1( "P1" );
    rt::parameter<int> p2( "P2" );
    rt::parameter<int> p3( "P1" );

    rt::parameters_store S;

    BOOST_TEST( S.is_empty() );

    S.add( p1 );
    S.add( p2 );

    BOOST_CHECK_THROW( S.add( p3 ), rt::ambiguous_param_definition );
    BOOST_TEST( !S.is_empty() );
    BOOST_TEST( S.all().size() == 2U );
    BOOST_TEST(( S.get("P1")->p_name.get() == "P1" ));
    BOOST_TEST(( S.get("P2")->p_name.get() == "P2" ));
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE_END()

// EOF
