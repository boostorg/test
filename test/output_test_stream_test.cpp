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
//  Description : tests output_test_stream test tool functionality
// ***************************************************************************

// Boost.Test
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
using boost::test_tools::output_test_stream;
using boost::unit_test::test_suite;

// STL
#include <iomanip>

//____________________________________________________________________________//

void
test_constructor()
{
    {
        output_test_stream output;
        BOOST_CHECK( !output.match_pattern() );
        BOOST_CHECK( output.is_empty() );
    }
    {
        output_test_stream output( (char const*)0 );
        BOOST_CHECK( !output.match_pattern() );
        BOOST_CHECK( output.is_empty() );
    }
    {
        output_test_stream output( "" );
        BOOST_CHECK( !output.match_pattern() );
        BOOST_CHECK( output.is_empty() );
    }
    {
        output_test_stream output( "%&^$%&$%" );
        BOOST_CHECK( !output.match_pattern() );
        BOOST_CHECK( output.is_empty() );
    }
    {
        output_test_stream output( "pattern.temp" );
        BOOST_CHECK( !output.match_pattern() );
        BOOST_CHECK( output.is_empty() );
    }
    {
        output_test_stream output( "pattern.temp2", false );
        BOOST_CHECK( output.match_pattern() );
        BOOST_CHECK( output.is_empty() );
    }
    {
        output_test_stream output( "pattern.temp2" );
        BOOST_CHECK( output.match_pattern() );
        BOOST_CHECK( output.is_empty() );
    }
}

//____________________________________________________________________________//

void
test_is_empty()
{
    output_test_stream output;
    BOOST_CHECK( output.is_empty() );

    output << 12345;
    BOOST_CHECK( !output.is_empty() );
    BOOST_CHECK( output.is_empty() );

    output << "";
    BOOST_CHECK( output.is_empty() );

    output << '\0';
    BOOST_CHECK( !output.is_empty( false ) );
    BOOST_CHECK( !output.is_empty() );
}

//____________________________________________________________________________//

void
test_check_length()
{
    output_test_stream output;
    BOOST_CHECK( output.check_length( 0 ) );

    output << "";
    BOOST_CHECK( output.check_length( 0 ) );

    output << '\0';
    BOOST_CHECK( output.check_length( 1 ) );

    output << 1220;
    BOOST_CHECK( output.check_length( 4 ) );

    output << "Text message";
    BOOST_CHECK( output.check_length( 12, false ) );
    BOOST_CHECK( output.check_length( 12 ) );

    output.width( 20 );
    output << "Text message";
    BOOST_CHECK( output.check_length( 20 ) );
}

//____________________________________________________________________________//

void
test_is_equal()
{
    output_test_stream output;
    BOOST_CHECK( output.is_equal( "" ) );

    output << 1;
    BOOST_CHECK( output.is_equal( "1" ) );

    output << "";
    BOOST_CHECK( output.is_equal( "" ) );

    output << '\0';
    BOOST_CHECK( output.is_equal( boost::unit_test::const_string( "", 1 ) ) );

    output << std::setw( 10 ) << "qwerty" << '\n';
    BOOST_CHECK( output.is_equal( "    qwerty\n" ) );

    std::string s( "test string" );

    output << s << std::endl;
    BOOST_CHECK( output.is_equal( "test string\n", false ) );
    
    output << s << std::endl;
    BOOST_CHECK( output.is_equal( "test string\ntest string\n" ) );

    char const* literal_string = "asdfghjkl";
    std::string substr1( literal_string, 5 );
    std::string substr2( literal_string+5, 4 );

    output << substr1;
    BOOST_CHECK( output.is_equal( boost::unit_test::const_string( literal_string, 5 ), false ) );

    output << substr2;
    BOOST_CHECK( output.is_equal( boost::unit_test::const_string( literal_string, 9 ) ) );
}

//____________________________________________________________________________//

void
test_match_pattern()
{
    {
        output_test_stream output( "pattern.test", false );
        
        output << "text1\n";
        BOOST_CHECK( output.match_pattern() );
        output << "text2\n";
        BOOST_CHECK( output.match_pattern() );
        output << "text3\n";
        BOOST_CHECK( output.match_pattern() );
    }
    {
        output_test_stream output( "pattern.test" );
        
        output << "text1\n";
        BOOST_CHECK( output.match_pattern() );
        output << "text2\n";
        BOOST_CHECK( output.match_pattern() );
        output << "text3\n";
        BOOST_CHECK( output.match_pattern() );
    }
    {
        output_test_stream output( "pattern.test" );
        
        output << "text4\n";
        BOOST_CHECK( !output.match_pattern() );
        output << "text2\n";
        BOOST_CHECK( output.match_pattern() );
        output << "text3\n";
        BOOST_CHECK( output.match_pattern() );
    }
    {
        output_test_stream output( "pattern.test" );
        
        output << "text\n";
        BOOST_CHECK( !output.match_pattern() );
        output << "text2\n";
        BOOST_CHECK( !output.match_pattern() );
        output << "text3\n";
        BOOST_CHECK( !output.match_pattern() );
    }
}

//____________________________________________________________________________//

test_suite*
init_unit_test_suite( int /*argc*/, char* /*argv*/[] ) {
    test_suite* test = BOOST_TEST_SUITE("ostream_test_stream test");

    test->add( BOOST_TEST_CASE( &test_constructor ) );
    test->add( BOOST_TEST_CASE( &test_is_empty ) );
    test->add( BOOST_TEST_CASE( &test_check_length ) );
    test->add( BOOST_TEST_CASE( &test_is_equal ) );
    test->add( BOOST_TEST_CASE( &test_match_pattern ) );

    return test;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.20  2005/05/11 05:07:57  rogeeff
//  licence update
//
//  Revision 1.19  2005/01/30 03:35:55  rogeeff
//  no message
//
//  Revision 1.18  2005/06/05 11:04:17  rogeeff
//  no message
//
//  Revision 1.17  2005/05/27 06:30:48  rogeeff
//  no message
//
//  Revision 1.16  2005/05/21 06:26:10  rogeeff
//  licence update
//
//  Revision 1.15  2005/05/11 11:05:06  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//  namespace names shortened
//
//  Revision 1.14  2003/12/01 00:42:37  rogeeff
//  prerelease cleaning
//

// ***************************************************************************

// EOF
