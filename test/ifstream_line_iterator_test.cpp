//  (C) Copyright Gennadiy Rozental 2001-2004.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : ifstream_line_iterator unit test
// *****************************************************************************

// Boost.Test
#include <boost/test/unit_test.hpp>

#include <boost/test/detail/iterator/ifstream_line_iterator.hpp>
#include <boost/test/detail/basic_cstring/compare.hpp>
#include <boost/test/detail/fixed_mapping.hpp>

namespace utf = boost::unit_test;
namespace tt  = boost::test_tools;
using utf::const_string;

//____________________________________________________________________________//

void test_default_delimeter()
{
    {
        utf::ifstream_line_iterator it( "./test_files/ifstream_line_iterator.tst1" );

        BOOST_CHECK( it != utf::ifstream_line_iterator() ); 

        BOOST_CHECK_EQUAL( *it, "acv ffg" ); 
        ++it;

        BOOST_CHECK_EQUAL( *it, "" ); 
        ++it;

        BOOST_CHECK_EQUAL( *it, " " ); 
        ++it;

        BOOST_CHECK_EQUAL( *it, "1" ); 
        ++it;

        BOOST_CHECK( it == utf::ifstream_line_iterator() ); 
    }
}

//____________________________________________________________________________//

void test_custom_delimeter()
{

    {
        utf::ifstream_line_iterator it( "./test_files/ifstream_line_iterator.tst2", '}' );

        BOOST_CHECK( it != utf::ifstream_line_iterator() ); 

        BOOST_CHECK_EQUAL( *it, "{ abc d " ); 
        ++it;

        BOOST_CHECK_EQUAL( *it, "\n{ d \n dsfg\n" ); 
        ++it;

        BOOST_CHECK_EQUAL( *it, "\n" ); 
        ++it;

        BOOST_CHECK( it == utf::ifstream_line_iterator() ); 
    }
}


//____________________________________________________________________________//

utf::test_suite*
init_unit_test_suite( int argc, char* argv[] )
{
    utf::test_suite* test= BOOST_TEST_SUITE( "ifstream line iterator unit test" );

    test->add( BOOST_TEST_CASE( &test_default_delimeter ) );
    test->add( BOOST_TEST_CASE( &test_custom_delimeter ) );

    return test;
}

// *****************************************************************************
// History :
//
// $Log$
// Revision 1.1  2004/05/21 06:25:21  rogeeff
// ifstream_line_iterator added
//
// *****************************************************************************

// EOF
