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
//  Description : unit test for runtime parameter framework
// ***************************************************************************

// Boost.Test
#define BOOST_TEST_MODULE Boost.Test CLA parser test
#include <boost/test/unit_test.hpp>
#include <boost/test/utils/runtime/parameter.hpp>
#include <boost/test/utils/runtime/cla/argv_traverser.hpp>
#include <boost/test/utils/runtime/cla/parser.hpp>
namespace utf = boost::unit_test;
namespace rt = boost::runtime;

#include <iostream>

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

    tr.skip( 2 );
    new_argc = tr.remainder();

    BOOST_TEST( new_argc == 2 );
    BOOST_TEST( argv[0] == "test.exe" );
    BOOST_TEST( argv[1] == " vb" );

    tr.skip( 3 );
    new_argc = tr.remainder();

    BOOST_TEST( new_argc == 1 );
    BOOST_TEST( argv[0] == "test.exe" );
}

BOOST_AUTO_TEST_SUITE_END()

//____________________________________________________________________________//
//____________________________________________________________________________//
//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE( test_parameter_specification,
                       * utf::depends_on("test_argv_traverser") )

BOOST_AUTO_TEST_CASE( test_param_construction )
{
    rt::parameter<int> p1( "P1" );

    BOOST_TEST( p1.p_name.get() == "P1" );
    BOOST_TEST( p1.p_description.get() == "" );
    BOOST_TEST( p1.p_env_var.get() == "" );
    BOOST_TEST( p1.p_optional );
    BOOST_TEST( !p1.p_repeatable );
    BOOST_TEST( !p1.p_has_optional_value );

    rt::parameter<int,rt::REQUIRED> p2( "P2", (
        rt::description = "123",
        rt::env_var = "E2"
    ));

    BOOST_TEST( p2.p_name.get() == "P2" );
    BOOST_TEST( p2.p_description.get() == "123" );
    BOOST_TEST( p2.p_env_var.get() == "E2" );
    BOOST_TEST( !p2.p_optional );
    BOOST_TEST( !p2.p_repeatable );
    BOOST_TEST( !p2.p_has_optional_value );

    rt::parameter<int,rt::REPEATABLE> p4( "P4", (
        rt::description = "123",
        rt::env_var = "E4",
        rt::optional_value = 5
    ));

    BOOST_TEST( p4.p_name.get() == "P4" );
    BOOST_TEST( p4.p_description.get() == "123" );
    BOOST_TEST( p4.p_env_var.get() == "E4" );
    BOOST_TEST( p4.p_optional );
    BOOST_TEST( p4.p_repeatable );
    BOOST_TEST( p4.p_has_optional_value );

    rt::option p5( "P5", (
        rt::description = "bool arg",
        rt::env_var = "E5"
    ));
    p5.add_cla_id( "-", "b", " " );

    BOOST_TEST( p5.p_name.get() == "P5" );
    BOOST_TEST( p5.p_description.get() == "bool arg" );
    BOOST_TEST( p5.p_env_var.get() == "E5" );
    BOOST_TEST( p5.p_optional );
    BOOST_TEST( !p5.p_repeatable );
    BOOST_TEST( p5.p_has_optional_value );

    rt::parameter<int> p3( "P3" );
    p3.add_cla_id( "/", "P3", ":" );
    p3.add_cla_id( "-", "p+p_p", " " );

    BOOST_TEST( p3.cla_ids().size() == 2U );
    BOOST_TEST( p3.cla_ids()[0].m_prefix == "/" );
    BOOST_TEST( p3.cla_ids()[0].m_full_name == "P3" );
    BOOST_TEST( p3.cla_ids()[0].m_value_separator == ":" );
    BOOST_TEST( p3.cla_ids()[1].m_prefix == "-" );
    BOOST_TEST( p3.cla_ids()[1].m_full_name == "p+p_p" );
    BOOST_TEST( p3.cla_ids()[1].m_value_separator == "" );

    BOOST_CHECK_THROW( p3.add_cla_id( "^", "p", " " ), rt::invalid_cla_id );
    BOOST_CHECK_THROW( p3.add_cla_id( " ", "p", " " ), rt::invalid_cla_id );
    BOOST_CHECK_THROW( p3.add_cla_id( "", "p", " " ), rt::invalid_cla_id );
    BOOST_CHECK_THROW( p3.add_cla_id( "-", "-abc", " " ), rt::invalid_cla_id );
    BOOST_CHECK_THROW( p3.add_cla_id( "-", "b c", " " ), rt::invalid_cla_id );
    BOOST_CHECK_THROW( p3.add_cla_id( "-", "", " " ), rt::invalid_cla_id );
    BOOST_CHECK_THROW( p3.add_cla_id( "-", "a", "-" ), rt::invalid_cla_id );
    BOOST_CHECK_THROW( p3.add_cla_id( "-", "a", "/" ), rt::invalid_cla_id );
    BOOST_CHECK_THROW( p3.add_cla_id( "-", "a", "" ), rt::invalid_cla_id );

    rt::parameter<int> p6( "P3", rt::optional_value = 1);
    BOOST_CHECK_THROW( p6.add_cla_id( "-", "a", " " ), rt::invalid_cla_id );
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

    BOOST_CHECK_THROW( S.add( p3 ), rt::duplicate_param );
    BOOST_TEST( !S.is_empty() );
    BOOST_TEST( S.all().size() == 2U );
    BOOST_TEST( S.get("P1")->p_name.get() == "P1" );
    BOOST_TEST( S.get("P2")->p_name.get() == "P2" );
    BOOST_CHECK_THROW( S.get("P3"), rt::unknown_param );
}

BOOST_AUTO_TEST_SUITE_END()

//____________________________________________________________________________//
//____________________________________________________________________________//
//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_param_trie_construction,
                      * utf::depends_on("test_parameter_specification") )
{
    // make sure this does not crash
    rt::parameters_store store;
    rt::cla::parser parser( store );

    rt::parameters_store store1;

    rt::parameter<int> p1( "P1" );
    p1.add_cla_id( "--", "param_one", " " );
    p1.add_cla_id( "-", "p1", " " );
    store1.add( p1 );

    rt::parameter<int> p2( "P2" );
    p2.add_cla_id( "--", "param_two", " " );
    p2.add_cla_id( "-", "p2", " " );
    store1.add( p2 );

    rt::parameter<int> p3( "P3" );
    p3.add_cla_id( "--", "another_", " " );
    p3.add_cla_id( "/", "p3", " " );
    store1.add( p3 );

    rt::parameter<int> p4( "P4" );
    p4.add_cla_id( "-", "param_one", " " );
    store1.add( p4 );

    rt::cla::parser parser1( store1 );

    rt::parameters_store store2;

    rt::parameter<int> p5( "P5" );
    p5.add_cla_id( "-", "paramA", " " );
    store2.add( p5 );

    rt::parameter<int> p6( "P6" );
    p6.add_cla_id( "-", "paramA", " " );
    store2.add( p6 );

    BOOST_CHECK_THROW( rt::cla::parser parser( store2 ), rt::conflicting_param );

    rt::parameters_store store3;

    rt::parameter<int> p7( "P7" );
    p7.add_cla_id( "-", "paramA", " " );
    store3.add( p7 );

    rt::parameter<int> p8( "P8" );
    p8.add_cla_id( "-", "param", " " );
    store3.add( p8 );

    BOOST_CHECK_THROW( rt::cla::parser parser( store3 ), rt::conflicting_param );

    rt::parameters_store store4;

    rt::parameter<int> p9( "P9" );
    p9.add_cla_id( "-", "param", " " );
    store4.add( p9 );

    rt::parameter<int> p10( "P10" );
    p10.add_cla_id( "-", "paramA", " " );
    store4.add( p10 );

    BOOST_CHECK_THROW( rt::cla::parser parser( store4 ), rt::conflicting_param );
}

//____________________________________________________________________________//
//____________________________________________________________________________//
//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_arguments_store )
{
    rt::arguments_store store;

    BOOST_TEST( store.size() == 0U );
    BOOST_TEST( !store.has( "P1" ) );
    BOOST_TEST( !store.has( "P2" ) );
    BOOST_TEST( !store.has( "P3" ) );
    BOOST_TEST( !store.has( "P4" ) );
    BOOST_TEST( !store.has( "P5" ) );

    store.set( "P1", 10 );
    store.set( "P2", std::string("abc") );
    store.set( "P3", rt::cstring("abc") );
    store.set( "P4", true );
    store.set( "P5", std::vector<int>( 1, 12 ) );

    BOOST_TEST( store.has( "P1" ) );
    BOOST_TEST( store.has( "P2" ) );
    BOOST_TEST( store.has( "P3" ) );
    BOOST_TEST( store.has( "P4" ) );
    BOOST_TEST( store.has( "P5" ) );
    BOOST_TEST( store.size() == 5U );

    BOOST_TEST( store.get<int>( "P1" ) == 10 );
    BOOST_TEST( store.get<std::string>( "P2" ) == "abc" );
    BOOST_TEST( store.get<rt::cstring>( "P3" ) == "abc" );
    BOOST_TEST( store.get<bool>( "P4" ) == true);
    BOOST_TEST( store.get<std::vector<int>>( "P5" ) == std::vector<int>( 1, 12 ) );

    store.set( "P1", 20 );
    BOOST_TEST( store.get<int>( "P1" ) == 20 );

    BOOST_CHECK_THROW( store.get<int>( "P0" ), rt::missing_argument );
    BOOST_CHECK_THROW( store.get<long>( "P1" ), rt::arg_type_mismatch );
}

//____________________________________________________________________________//
//____________________________________________________________________________//
//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE( test_cla_parsing,
                       * utf::depends_on("test_argv_traverser")
                       * utf::depends_on("test_parameter_specification")
                       * utf::depends_on("test_arguments_store") )

BOOST_AUTO_TEST_CASE( test_basic_parsing )
{
    rt::parameters_store param_store;

    rt::parameter<std::string> p1( "P1" );
    p1.add_cla_id( "--", "param_one", "=" );
    p1.add_cla_id( "-", "p1", " " );
    param_store.add( p1 );

    rt::parameter<std::string> p2( "P2" );
    p2.add_cla_id( "--", "param_two", "=" );
    p2.add_cla_id( "-", "p2", " " );
    param_store.add( p2 );

    rt::cla::parser parser( param_store );

    char const* argv1[] = { "test.exe" };
    rt::arguments_store args_store1;

    parser.parse( sizeof(argv1)/sizeof(char const*), (char**)argv1, args_store1 );

    BOOST_TEST( args_store1.size() == 0U );

    char const* argv2[] = { "test.exe", "--param_one=abc" };
    rt::arguments_store args_store2;

    parser.parse( sizeof(argv2)/sizeof(char const*), (char**)argv2, args_store2 );

    BOOST_TEST( args_store2.size() == 1U );
    BOOST_TEST( args_store2.has( "P1" ) );
    BOOST_TEST( args_store2.get<std::string>( "P1" ) == "abc" );

    char const* argv3[] = { "test.exe", "--param_two=12" };
    rt::arguments_store args_store3;

    parser.parse( sizeof(argv3)/sizeof(char const*), (char**)argv3, args_store3 );

    BOOST_TEST( args_store3.size() == 1U );
    BOOST_TEST( args_store3.has( "P2" ) );
    BOOST_TEST( args_store3.get<std::string>( "P2" ) == "12" );

    char const* argv4[] = { "test.exe", "-p1", "aaa", "-p2", "37" };
    rt::arguments_store args_store4;

    parser.parse( sizeof(argv4)/sizeof(char const*), (char**)argv4, args_store4 );

    BOOST_TEST( args_store4.size() == 2U );
    BOOST_TEST( args_store4.has( "P1" ) );
    BOOST_TEST( args_store4.get<std::string>( "P1" ) == "aaa" );
    BOOST_TEST( args_store4.has( "P2" ) );
    BOOST_TEST( args_store4.get<std::string>( "P2" ) == "37" );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_typed_argument_parsing )
{
    rt::parameters_store param_store;

    rt::parameter<double> p1( "P1" );
    p1.add_cla_id( "--", "param_one", "=" );
    p1.add_cla_id( "-", "p1", " " );
    param_store.add( p1 );

    rt::parameter<int> p2( "P2" );
    p2.add_cla_id( "--", "param_two", "=" );
    p2.add_cla_id( "-", "p2", " " );
    param_store.add( p2 );

    rt::option p3( "P3" );
    p3.add_cla_id( "--", "third", "=" );
    p3.add_cla_id( "-", "p3", " " );
    param_store.add( p3 );

    rt::parameter<rt::cstring> p4( "P4" );
    p4.add_cla_id( "--", "another", "=" );
    p4.add_cla_id( "-", "p4", " " );
    param_store.add( p4 );

    rt::cla::parser parser( param_store );

    char const* argv1[] = { "test.exe", "--another=some thing", "-p1", "1.2", "-p2", "37", "--third=Y" };
    rt::arguments_store args_store1;

    parser.parse( sizeof(argv1)/sizeof(char const*), (char**)argv1, args_store1 );

    BOOST_TEST( args_store1.size() == 4U );
    BOOST_TEST( args_store1.has( "P1" ) );
    BOOST_TEST( args_store1.get<double>( "P1" ) == 1.2 );
    BOOST_TEST( args_store1.has( "P2" ) );
    BOOST_TEST( args_store1.get<int>( "P2" ) == 37 );
    BOOST_TEST( args_store1.has( "P3" ) );
    BOOST_TEST( args_store1.get<bool>( "P3" ) );
    BOOST_TEST( args_store1.has( "P4" ) );
    BOOST_TEST( args_store1.get<rt::cstring>( "P4" ) == "some thing" );

    char const* argv2[] = { "test.exe", "-p3" };
    rt::arguments_store args_store2;

    parser.parse( sizeof(argv2)/sizeof(char const*), (char**)argv2, args_store2 );
    BOOST_TEST( args_store2.size() == 1U );
    BOOST_TEST( args_store2.has( "P3" ) );
    BOOST_TEST( args_store2.get<bool>( "P3" ) );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_vector_argument_parsing )
{
    rt::parameters_store param_store;

    rt::parameter<std::vector<std::string>> p1( "P1" );
    p1.add_cla_id( "--", "param_one", "=" );
    p1.add_cla_id( "-", "p1", " " );
    param_store.add( p1 );

    rt::parameter<std::vector<int>> p2( "P2" );
    p2.add_cla_id( "--", "param_two", "=" );
    p2.add_cla_id( "-", "p2", " " );
    param_store.add( p2 );

    rt::cla::parser parser( param_store );

    char const* argv1[] = { "test.exe", "--param_one=a,b,c", "-p2", "1,2,3" };
    rt::arguments_store args_store1;

    parser.parse( sizeof(argv1)/sizeof(char const*), (char**)argv1, args_store1 );

    BOOST_TEST( args_store1.size() == 2U );
    BOOST_TEST( args_store1.has( "P1" ) );
    std::vector<std::string> P1_expected{"a", "b", "c"};
    BOOST_TEST( args_store1.get<std::vector<std::string>>( "P1" ) == P1_expected );
    BOOST_TEST( args_store1.has( "P2" ) );
    std::vector<int> P2_expected{1, 2, 3};
    BOOST_TEST( args_store1.get<std::vector<int>>( "P2" ) == P2_expected );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_parameter_name_guessing )
{
    rt::parameters_store param_store;

    rt::parameter<int> p1( "P1" );
    p1.add_cla_id( "--", "param_one", "=" );
    p1.add_cla_id( "-", "one", " " );
    param_store.add( p1 );

    rt::parameter<int> p2( "P2" );
    p2.add_cla_id( "--", "param_two", "=" );
    p2.add_cla_id( "-", "two", " " );
    param_store.add( p2 );

    rt::cla::parser parser( param_store );

    char const* argv1[] = { "test.exe", "--param_o=1", "-t", "2" };
    rt::arguments_store args_store1;

    parser.parse( sizeof(argv1)/sizeof(char const*), (char**)argv1, args_store1 );

    BOOST_TEST( args_store1.size() == 2U );
    BOOST_TEST( args_store1.has( "P1" ) );
    BOOST_TEST( args_store1.has( "P2" ) );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_repeatable_parameters )
{
    rt::parameters_store param_store;

    rt::parameter<int,rt::REPEATABLE> p1( "P1" );
    p1.add_cla_id( "--", "param_one", "=" );
    p1.add_cla_id( "-", "one", " " );
    param_store.add( p1 );

    rt::cla::parser parser( param_store );
    rt::arguments_store args_store;

    char const* argv[] = { "test.exe", "-one", "1", "-one", "2" };
    parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store );

    BOOST_TEST( args_store.size() == 1U );
    BOOST_TEST( args_store.has( "P1" ) );

    std::vector<int> P1_expected{1, 2};
    BOOST_TEST( args_store.get<std::vector<int>>( "P1" ) == P1_expected );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_parameter_with_optional_value )
{
    rt::parameters_store param_store;

    rt::parameter<int> p1( "P1", rt::optional_value = 5 );
    p1.add_cla_id( "--", "param_one", "=" );
    param_store.add( p1 );

    rt::cla::parser parser( param_store );

    rt::arguments_store args_store1;
    char const* argv1[] = { "test.exe", "--param_one=1" };
    parser.parse( sizeof(argv1)/sizeof(char const*), (char**)argv1, args_store1 );

    BOOST_TEST( args_store1.size() == 1U );
    BOOST_TEST( args_store1.has( "P1" ) );
    BOOST_TEST( args_store1.get<int>( "P1" ) == 1 );

    rt::arguments_store args_store2;
    char const* argv2[] = { "test.exe", "--param_one" };
    parser.parse( sizeof(argv2)/sizeof(char const*), (char**)argv2, args_store2 );

    BOOST_TEST( args_store2.size() == 1U );
    BOOST_TEST( args_store2.has( "P1" ) );
    BOOST_TEST( args_store2.get<int>( "P1" ) == 5 );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_validations )
{
    rt::parameters_store param_store;

    rt::parameter<int> p1( "P1" );
    p1.add_cla_id( "--", "param_one", "=" );
    p1.add_cla_id( "-", "one", " " );
    param_store.add( p1 );

    rt::parameter<int> p2( "P2" );
    p2.add_cla_id( "--", "param_two", "=" );
    p2.add_cla_id( "-", "two", " " );
    param_store.add( p2 );

    rt::option p3( "P3" );
    p3.add_cla_id( "--", "option", "=" );
    param_store.add( p3 );

    rt::cla::parser parser( param_store );
    rt::arguments_store args_store;

    {
        char const* argv[] = { "test.exe", "param_one=1" };
        BOOST_CHECK_THROW( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), rt::format_error );
    }

    {
        char const* argv[] = { "test.exe", "/param_one=1" };
        BOOST_CHECK_THROW( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), rt::format_error );
    }

    {
        char const* argv[] = { "test.exe", "---param_one=1" };
        BOOST_CHECK_THROW( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), rt::format_error );
    }

    {
        char const* argv[] = { "test.exe", "--=1" };
        BOOST_CHECK_THROW( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), rt::format_error );
    }

    {
        char const* argv[] = { "test.exe", "--param_one:1" };
        BOOST_CHECK_THROW( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), rt::format_error );
    }

    {
        char const* argv[] = { "test.exe", "--param_one=" };
        BOOST_CHECK_THROW( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), rt::format_error );
    }

    {
        char const* argv[] = { "test.exe", "--param_one=1", "--param_one=2" };
        BOOST_CHECK_THROW( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), rt::duplicate_arg );
    }

    {
        char const* argv[] = { "test.exe", "--param=1" };
        BOOST_CHECK_THROW( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), rt::ambiguous_param );
    }

    {
        char const* argv[] = { "test.exe", "=1" };
        BOOST_CHECK_THROW( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), rt::format_error );
    }

    {
        char const* argv[] = { "test.exe", "--opt=Yeah" };
        BOOST_CHECK_THROW( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), rt::format_error );
    }
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_unrecognized_param_suggestions )
{
    rt::parameters_store param_store;

    rt::parameter<int> p1( "P1" );
    p1.add_cla_id( "--", "param_one", "=" );
    p1.add_cla_id( "-", "p1", " " );
    param_store.add( p1 );

    rt::parameter<int> p2( "P2" );
    p2.add_cla_id( "--", "param_two", "=" );
    param_store.add( p2 );

    rt::parameter<int> p3( "P3" );
    p3.add_cla_id( "--", "param_three", "=" );
    param_store.add( p3 );

    rt::cla::parser parser( param_store );
    rt::arguments_store args_store;

    {
        char const* argv[] = { "test.exe", "--laram_one=1" };
        BOOST_CHECK_EXCEPTION( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), 
                               rt::unrecognized_param, 
                               []( rt::unrecognized_param const& ex ) -> bool {
                                    return ex.m_typo_candidates == std::vector<rt::cstring>{"param_one"};                        
                               });
    }

    {
        char const* argv[] = { "test.exe", "--paran_one=1" };
        BOOST_CHECK_EXCEPTION( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), 
                               rt::unrecognized_param, 
                               []( rt::unrecognized_param const& ex ) -> bool {
                                    return ex.m_typo_candidates == std::vector<rt::cstring>{"param_one"};                        
                               });
    }

    {
        char const* argv[] = { "test.exe", "--param_onw=1" };
        BOOST_CHECK_EXCEPTION( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), 
                               rt::unrecognized_param, 
                               []( rt::unrecognized_param const& ex ) -> bool {
                                    return ex.m_typo_candidates == std::vector<rt::cstring>{"param_one"};                        
                               });
    }

    {
        char const* argv[] = { "test.exe", "--param_to=1" };
        BOOST_CHECK_EXCEPTION( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), 
                               rt::unrecognized_param, 
                               ([]( rt::unrecognized_param const& ex ) -> bool {
                                    return ex.m_typo_candidates == std::vector<rt::cstring>{"param_three", "param_two"};                        
                               }));
    }

    {
        char const* argv[] = { "test.exe", "--paramtwo=1" };
        BOOST_CHECK_EXCEPTION( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), 
                               rt::unrecognized_param, 
                               ([]( rt::unrecognized_param const& ex ) -> bool {
                                    return ex.m_typo_candidates == std::vector<rt::cstring>{"param_two"};                        
                               }));
    }

    {
        char const* argv[] = { "test.exe", "--parum_=1" };
        BOOST_CHECK_EXCEPTION( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), 
                               rt::unrecognized_param, 
                               ([]( rt::unrecognized_param const& ex ) -> bool {
                                    return ex.m_typo_candidates == std::vector<rt::cstring>{};                        
                               }));
    }

    {
        char const* argv[] = { "test.exe", "--param__one=1" };
        BOOST_CHECK_EXCEPTION( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), 
                               rt::unrecognized_param, 
                               ([]( rt::unrecognized_param const& ex ) -> bool {
                                    return ex.m_typo_candidates == std::vector<rt::cstring>{"param_one"};                        
                               }));
    }

    {
        char const* argv[] = { "test.exe", "--param_twoo=1" };
        BOOST_CHECK_EXCEPTION( parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store ), 
                               rt::unrecognized_param, 
                               ([]( rt::unrecognized_param const& ex ) -> bool {
                                    return ex.m_typo_candidates == std::vector<rt::cstring>{"param_two"};                        
                               }));
    }
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_end_of_params )
{
    rt::parameters_store param_store;

    rt::parameter<int> p1( "P1" );
    p1.add_cla_id( "--", "param_one", "=" );
    p1.add_cla_id( "-", "p1", " " );
    param_store.add( p1 );

    rt::parameter<int> p2( "P2" );
    p2.add_cla_id( "--", "param_two", "=" );
    param_store.add( p2 );

    rt::cla::parser parser( param_store, rt::end_of_params = "--" );

    {
        rt::arguments_store args_store;
        char const* argv[] = { "test.exe", "--param_one=1", "--", "/abc" };
        int new_argc = parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store );

        BOOST_TEST( args_store.size() == 1U );
        BOOST_TEST( args_store.has( "P1" ) );
        BOOST_TEST( args_store.get<int>( "P1" ) == 1 );
        BOOST_TEST( new_argc == 2 );
    }

    {
        rt::arguments_store args_store;           
        char const* argv[] = { "test.exe", "-p1", "1", "--", "--param_two=2" };
        int new_argc = parser.parse( sizeof(argv)/sizeof(char const*), (char**)argv, args_store );

        BOOST_TEST( args_store.size() == 1U );
        BOOST_TEST( args_store.has( "P1" ) );
        BOOST_TEST( !args_store.has( "P2" ) );
        BOOST_TEST( args_store.get<int>( "P1" ) == 1 );
        BOOST_TEST( new_argc == 2 );
    }
}

BOOST_AUTO_TEST_SUITE_END()

// EOF
