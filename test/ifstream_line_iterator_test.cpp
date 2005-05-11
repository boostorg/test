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
//  Description : ifstream_line_iterator unit test
// *****************************************************************************

// Boost.Test
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

#include <boost/test/utils/iterator/ifstream_line_iterator.hpp>

namespace utf = boost::unit_test;

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_default_delimeter )
{
    utf::ifstream_line_iterator it( utf::auto_unit_test_suite()->argc <= 1
                                        ? "./test_files/ifstream_line_iterator.tst1"
                                        : utf::auto_unit_test_suite()->argv[1] );

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

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_custom_delimeter )
{
    utf::ifstream_line_iterator it( utf::auto_unit_test_suite()->argc <= 2 
                                        ? "./test_files/ifstream_line_iterator.tst2"
                                        : utf::auto_unit_test_suite()->argv[2], '}' );

    BOOST_CHECK( it != utf::ifstream_line_iterator() ); 

    BOOST_CHECK_EQUAL( *it, "{ abc d " ); 
    ++it;

    BOOST_CHECK_EQUAL( *it, "\n{ d \n dsfg\n" ); 
    ++it;

    BOOST_CHECK_EQUAL( *it, "\n" ); 
    ++it;

    BOOST_CHECK( it == utf::ifstream_line_iterator() ); 
}


//____________________________________________________________________________//

// *****************************************************************************
// History :
//
// $Log$
// Revision 1.6  2005/05/11 05:07:57  rogeeff
// licence update
//
// Revision 1.5  2005/03/22 07:14:44  rogeeff
// no message
//
// *****************************************************************************

// EOF
